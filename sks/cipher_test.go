package main

import (
	"bytes"
	"io/ioutil"
	"testing"
)

func TestFakeNetworkAB(t *testing.T) {
	network := bytes.NewBuffer(nil)

	// A nacita obrazok zo suboru
	img, _ := ioutil.ReadFile("zadanie.jpg")

	// A pocita Hash
	hash := hash32(img)

	// A vytvory vstup pre sifru msg = img + hash
	var msg []byte
	msg = append(msg, img...)
	msg = append(msg, hash...)

	// A vytvori kluc a sifruje
	key := []byte("Strong random key")
	enc := CipherEncrypt(key, msg)

	// A posiela zasifrovane cez siet
	network.Write(enc)

	// B cita zo siete
	recvMsg, _ := ioutil.ReadAll(network)

	// scenar 6
	// B pozna kluc a desifruje
	decMsg := CipherDecrypt(key, recvMsg)

	// B pozrie hash z desifrovaneho a porovna
	recvHash := decMsg[len(decMsg)-4:]
	decMsg = decMsg[:len(decMsg)-4]

	// ak sa hashe nerovnaju bola porusena integrita
	if !bytes.Equal(recvHash, hash32(decMsg)) {
		t.Fatal("B: bola porusena integrita")
	}

	// scenar 7
	// B pozna kluc a desifruje
	decMsg2 := CipherDecrypt(key, recvMsg)

	// B meni jeden bajt a desifruje
	decMsg2[0] ^= decMsg2[0]
	recvHash2 := decMsg2[:len(decMsg2)-4]
	decMsg2 = decMsg2[:len(decMsg2)-4]

	// ak sa hashe rovnaju to nie je dobre
	if bytes.Equal(recvHash2, hash32(decMsg2)) {
		t.Fatal("B: bola porusena integrita a hash je stale dobry :(")
	}
}

func TestCipherImage(t *testing.T) {
	key := []byte("My key")
	msg, _ := ioutil.ReadFile("zadanie.jpg")

	enc := CipherEncrypt(key, msg)
	dec := CipherDecrypt(key, enc)

	if bytes.Equal(msg, enc) {
		t.Fatal("plain text equals encrypted text")
	}

	if !bytes.Equal(msg, dec) {
		t.Fatal("plain text does not equal decrypted text")
	}
}

func TestCipher(t *testing.T) {

	key := []byte("VERY GOOD KEY")
	msg := []byte("Martin Elias")

	enc := CipherEncrypt(key, msg)
	dec := CipherDecrypt(key, enc)

	if bytes.Equal(msg, enc) {
		t.Fatal("plain text equals encrypted text")
	}

	if !bytes.Equal(msg, dec) {
		t.Fatal("plain text does not equal decrypted text")
	}

}
