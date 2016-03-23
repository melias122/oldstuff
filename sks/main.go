package main

import (
	"flag"
	"fmt"
	"os"
)

var (
	genkey  = flag.String("genkey", "", "Generates RSA or ElGamal key in OpenPGP binary format (rsa/elgamal)")
	keysize = flag.Int("size", 2048, "Specify key size 1024/2048/4096 for RSA/ElGamal")
	nprimes = flag.Int("primes", 2, "Specify number of primes for RSA")
)

func die(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(-1)
	}
}

func main() {

	flag.Parse()

	switch *genkey {
	case "":
		// do nothing
	case "rsa":
		rsa, err := RSAGenerateKey(*nprimes, *keysize)
		die(err)
		gpg := NewRSAOpenPGPPrivateKey(rsa)
		err = gpg.Serialize(os.Stdout)
		die(err)
		return

	case "elgamal":
		eg, err := ElGamalGenerateKey(*keysize)
		die(err)
		gpg2 := NewElGamalPGPPrivateKey(eg)
		err = gpg2.Serialize(os.Stdout)
		die(err)
		return

		// default:
		// flag.Usage()
		// return
	}
	flag.Usage()
}

// Poznamky k GPG

// 1. import private kluca, nefunguje cez gpg --import private.key
// cat private.key > ~/.gnupg/secring.gpg

// 2. import public kluca
// gpgsplit --no-split --verbose --secret-to-public private.key > ~/.gnupg/pubring.gpg

// 3. sifrovanie
// gpg --sign subor

// 4. desifrovanie
// gpg --decrypt subor.gpg
