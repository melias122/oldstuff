package main

import (
	"bytes"
	"math/big"
	"testing"
)

func TestRSA3(t *testing.T) {
	key, err := RSAGenerateKey(3, 2048)
	if err != nil {
		panic(err)
	}

	msg := []byte("Martin Elias")

	ct := RSAEncrypt(new(big.Int), &key.PublicKey, new(big.Int).SetBytes(msg))
	pt := RSADecrypt(new(big.Int), key, ct)

	if !bytes.Equal(msg, pt.Bytes()) {
		t.Fatal("Expected: %s, got: %s", msg, pt.Bytes())
	}
}
