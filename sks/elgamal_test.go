package main

import (
	"math/big"
	"testing"
)

func TestElGamal(t *testing.T) {
	priv, _ := ElGamalGenerateKey(2048)

	msg := new(big.Int).SetBytes([]byte("Martin Elias"))

	c1, c2 := ElGamalEncrypt(&priv.PublicKeyEG, msg)
	msg2 := ElGamalDecrypt(priv, c1, c2)

	if msg.Cmp(msg2) != 0 {
		t.Fatal("ElGamal: encryption/decription failed")
	}
}
