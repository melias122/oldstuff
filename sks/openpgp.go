package main

import (
	"bytes"
	"io"
	"math/big"
	"time"
)

type OpenPGPPublicKey struct {
	typ    byte       // 1 - rsa, 16 - elgamal
	public []*big.Int // N, E for RSA, P, G, Y for ElGamal
}

type OpenPGPPrivateKey struct {
	OpenPGPPublicKey
	// D, prime[1], prime[0], Qinv for RSA
	// X for ElGamal
	private []*big.Int
}

func NewRSAOpenPGPPrivateKey(rsa *PrivateKey) OpenPGPPrivateKey {
	return OpenPGPPrivateKey{
		OpenPGPPublicKey: OpenPGPPublicKey{
			typ: 1,
			public: []*big.Int{
				new(big.Int).Set(rsa.N),
				new(big.Int).Set(rsa.E),
			},
		},
		private: []*big.Int{
			new(big.Int).Set(rsa.D),
			new(big.Int).Set(rsa.Primes[1]),
			new(big.Int).Set(rsa.Primes[0]),
			new(big.Int).Set(rsa.Qinv),
		},
	}
}

func NewElGamalPGPPrivateKey(elgamal *PrivateKeyEG) OpenPGPPrivateKey {
	return OpenPGPPrivateKey{
		OpenPGPPublicKey: OpenPGPPublicKey{
			typ: 16,
			public: []*big.Int{
				new(big.Int).Set(elgamal.P),
				new(big.Int).Set(elgamal.G),
				new(big.Int).Set(elgamal.Y),
			},
		},
		private: []*big.Int{
			new(big.Int).Set(elgamal.X),
		},
	}
}

func (p OpenPGPPrivateKey) Serialize(w io.Writer) (err error) {

	// public part
	buf := bytes.NewBuffer(nil)
	err = serializePublicPart(buf, p.OpenPGPPublicKey)
	if err != nil {
		return
	}
	buf.WriteByte(0) // no encryption

	// private part
	privateKeyBuf := bytes.NewBuffer(nil)
	if err = serializePrivatePart(privateKeyBuf, p); err != nil {
		return
	}
	ptype := uint8(5) // packet type private key
	contents := buf.Bytes()
	privateKeyBytes := privateKeyBuf.Bytes()

	err = serializeHeader(w, ptype, len(contents)+len(privateKeyBytes)+2)
	if err != nil {
		return
	}
	_, err = w.Write(contents)
	if err != nil {
		return
	}
	_, err = w.Write(privateKeyBytes)
	if err != nil {
		return
	}

	checksum := mod64kHash(privateKeyBytes)
	var checksumBytes [2]byte
	checksumBytes[0] = byte(checksum >> 8)
	checksumBytes[1] = byte(checksum)
	_, err = w.Write(checksumBytes[:])
	if err != nil {
		return err
	}
	return
}

func serializePublicPart(w io.Writer, p OpenPGPPublicKey) (err error) {
	var buf [6]byte
	buf[0] = 4
	t := uint32(time.Now().Unix())
	buf[1] = byte(t >> 24)
	buf[2] = byte(t >> 16)
	buf[3] = byte(t >> 8)
	buf[4] = byte(t)
	buf[5] = p.typ

	_, err = w.Write(buf[:])
	if err != nil {
		return
	}
	return writeBigs(w, p.public...)
}

func serializePrivatePart(w io.Writer, priv OpenPGPPrivateKey) error {
	return writeBigs(w, priv.private...)
}

func serializeHeader(w io.Writer, ptype uint8, length int) (err error) {
	var buf [6]byte
	var n int

	buf[0] = 0x80 | 0x40 | byte(ptype)
	if length < 192 {
		buf[1] = byte(length)
		n = 2
	} else if length < 8384 {
		length -= 192
		buf[1] = 192 + byte(length>>8)
		buf[2] = byte(length)
		n = 3
	} else {
		buf[1] = 255
		buf[2] = byte(length >> 24)
		buf[3] = byte(length >> 16)
		buf[4] = byte(length >> 8)
		buf[5] = byte(length)
		n = 6
	}

	_, err = w.Write(buf[:n])
	return
}

func writeBigs(w io.Writer, bigs ...*big.Int) (err error) {
	for _, b := range bigs {
		err = writeBig(w, b)
		if err != nil {
			return
		}
	}
	return
}

func writeBig(w io.Writer, b *big.Int) (err error) {
	bytes := b.Bytes()
	bitLength := uint16(b.BitLen())
	_, err = w.Write([]byte{byte(bitLength >> 8), byte(bitLength)})
	if err == nil {
		_, err = w.Write(bytes)
	}
	return
}

func mod64kHash(d []byte) uint16 {
	var h uint16
	for _, b := range d {
		h += uint16(b)
	}
	return h
}
