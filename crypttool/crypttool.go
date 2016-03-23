package main

import (
	"bufio"
	"bytes"
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"crypto/sha256"
	"errors"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"os"
)

var (
	filePath         string
	encrypt, decrypt bool
)

func init() {
	flag.BoolVar(&encrypt, "e", false, "encrypt")
	flag.BoolVar(&decrypt, "d", false, "decrypt")
	flag.StringVar(&filePath, "f", "", "path to file we want encrypt/decrypt")
}

func usage() {
	fmt.Println(
		`Author: Martin Elias

		Program cryptool sluzi ako nastroj na sifrovanie suborov.
		Subor je zasifrovany pomocou AES-128 (CFB) a zo suboru
		sa vypocitava hash pomocou SHA-256.

		Pri sifrovani sa automaticky vytvara kluc (keyfile)
		a odtlacok (hashfile). Kluc je potrebne bezpecne uchovat.
		Sifrovanie: 	./crypttool -e -f plaintext

		Pri desifrovani je nutne mat spolu zo zasifrovanim suborom
		v zlozke subor s klucom (keyfile) a aj s odtlackom (hashfile).
		Desifrovanie: 	./crypttool -d -f ciphertext
		`)
	flag.Usage()
	os.Exit(1)
}

func Error(err error) {
	if err != nil {
		fmt.Printf("Error: %s\n", err)
		os.Exit(1)
	}
}

type KeySize int

const (
	Key16 KeySize = 16
	Key24 KeySize = 24
	Key32 KeySize = 32

	KeyDefault = Key16
)

func RandomKeySize(size KeySize) []byte {
	key := make([]byte, size)
	_, err := rand.Read(key)
	if err != nil {
		panic(err)
	}
	return key
}

func RandomKey() []byte {
	return RandomKeySize(KeyDefault)
}

func InitializationVector(blockSize int) []byte {
	iv := make([]byte, blockSize)
	_, err := rand.Read(iv)
	if err != nil {
		panic(err)
	}
	return iv
}

func Encrypt(in *os.File) {

	out, err := os.Create("ciphertext")
	Error(err)
	defer out.Close()

	// We will write to writter
	reader := bufio.NewReader(in)
	writer := bufio.NewWriter(out)

	// Generate random key of size 16 bytes
	key := RandomKey()

	// Sha256 computes hash
	Sha256 := sha256.New()

	// Create AES cipher with given random key
	aes, _ := aes.NewCipher(key)

	// Create initialization vector
	iv := InitializationVector(aes.BlockSize())
	_, err = writer.Write(iv)
	Error(err)

	// Aes CFB mode
	aesCFB := cipher.NewCFBEncrypter(aes, iv)

	var (
		plaintext  = make([]byte, aes.BlockSize())
		ciphertext = make([]byte, aes.BlockSize())
		done       bool
	)
	for !done {
		n, err := reader.Read(plaintext)
		if err == io.EOF {
			done = true
		} else if err != nil {
			Error(err)
		}
		_, err = Sha256.Write(plaintext[:n])
		Error(err)
		aesCFB.XORKeyStream(ciphertext, plaintext[:n])

		_, err = writer.Write(ciphertext[:n])
		Error(err)
	}
	writer.Flush()

	// Write key to file
	ioutil.WriteFile("keyfile", key, 0600)

	// Write hash to file
	ioutil.WriteFile("hashfile", Sha256.Sum(nil), 0644)
}

func Decrypt(in *os.File) {
	out, err := os.Create("plaintext")
	Error(err)
	defer out.Close()

	reader := bufio.NewReader(in)
	writer := bufio.NewWriter(out)

	// Read key
	keyfile, err := os.Open("keyfile")
	Error(err)
	defer keyfile.Close()

	key := make([]byte, KeyDefault)
	n, err := io.ReadFull(keyfile, key)
	Error(err)
	if n != len(key) {
		Error(errors.New("Unexpected key length"))
	}

	// Sha256 computes hash
	Sha256 := sha256.New()

	// Create AES cipher with given random key
	aes, _ := aes.NewCipher(key)

	// Read initialization vector
	iv := make([]byte, aes.BlockSize())
	// n, err = io.ReadFull(in, iv)
	n, err = reader.Read(iv)
	Error(err)
	if n != len(iv) {
		Error(errors.New("Can't read initialization vector"))
	}

	// Aes CFB mode
	aesCFB := cipher.NewCFBDecrypter(aes, iv)

	var (
		plaintext  = make([]byte, aes.BlockSize())
		ciphertext = make([]byte, aes.BlockSize())
		done       bool
	)
	for !done {
		// n, err := io.ReadFull(in, ciphertext)
		n, err := reader.Read(ciphertext)
		if err == io.EOF {
			done = true
		} else if err != nil {
			Error(err)
		}
		aesCFB.XORKeyStream(plaintext, ciphertext[:n])
		_, err = Sha256.Write(plaintext[:n])
		Error(err)

		_, err = writer.Write(plaintext[:n])
		Error(err)
	}
	writer.Flush()

	// open hashfile
	hashfile, err := os.Open("hashfile")
	Error(err)
	defer hashfile.Close()

	hash := make([]byte, sha256.Size)
	n, err = io.ReadFull(hashfile, hash)
	Error(err)
	if n != len(hash) {
		Error(errors.New("Can't read hashfile"))
	}

	if !bytes.Equal(Sha256.Sum(nil), hash) {
		fmt.Println("Integrity: corrupted file")
	}
}

func main() {
	flag.Parse()
	if flag.NFlag() != 2 {
		usage()
	}

	if encrypt && decrypt {
		usage()
	}

	if encrypt {
		f, err := os.Open(filePath)
		Error(err)
		defer f.Close()
		Encrypt(f)
	} else if decrypt {
		f, err := os.Open(filePath)
		Error(err)
		defer f.Close()
		Decrypt(f)
	} else {
		usage()
	}
}
