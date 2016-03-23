package main

import "math/big"

const blockSize = 32
const rounds = 7

func CipherEncrypt(key []byte, msg []byte) []byte {

	// add pading, compute bits in last block
	// 0 means block is aligned, so add padding block
	// 8 means one byte in last block
	// 16 etc
	// 24 one byte left in last block. add one more block
	// msg || padding
	// padding = zeroes || numzeros
	var pad []byte
	switch (len(msg) * 8) % blockSize {
	case 0:
		pad = []byte{0, 0, 0, 3}
	case 8:
		pad = []byte{0, 0, 2}
	case 16:
		pad = []byte{0, 1}
	case 24:
		pad = []byte{0, 0, 0, 0, 4}
	}

	// add padding
	msg = append(msg, pad...)

	// generate key from LFSR 32 bit size
	// seed it to key value
	keystream, _ := NewLFSR(blockSize)
	keystream.Seed(new(big.Int).SetBytes(key))

	var keys [rounds]uint16

	out := make([]byte, len(msg))
	for i := 0; i < len(msg); i += 4 {

		// generate round keys
		for j := range keys {
			keys[j] = uint16(keystream.RandInt().Uint64())
		}

		// convert bytes to uint16
		e := [2]uint16{
			uint16(msg[i])<<8 | uint16(msg[i+1]),
			uint16(msg[i+2])<<8 | uint16(msg[i+3]),
		}

		// encrypt
		e = encrypt(e, keys)

		// convert to bytes
		out[i] = byte(e[0] >> 8)
		out[i+1] = byte(e[0])
		out[i+2] = byte(e[1] >> 8)
		out[i+3] = byte(e[1])
	}
	return out
}

func CipherDecrypt(key []byte, msg []byte) []byte {

	keystream, _ := NewLFSR(blockSize)
	keystream.Seed(new(big.Int).SetBytes(key))

	var keys [rounds]uint16

	out := make([]byte, len(msg))
	for i := 0; i < len(msg); i += 4 {

		for j := range keys {
			keys[j] = uint16(keystream.RandInt().Uint64())
		}

		e := [2]uint16{
			uint16(msg[i])<<8 | uint16(msg[i+1]),
			uint16(msg[i+2])<<8 | uint16(msg[i+3]),
		}

		e = decrypt(e, keys)

		out[i] = byte(e[0] >> 8)
		out[i+1] = byte(e[0])
		out[i+2] = byte(e[1] >> 8)
		out[i+3] = byte(e[1])
	}

	// remove padding
	switch out[len(out)-1] {
	case 1:
		out = out[:len(out)-2]
	case 2:
		out = out[:len(out)-3]
	case 3:
		out = out[:len(out)-4]
	case 4:
		out = out[:len(out)-5]
	}
	return out
}

func encrypt(data [2]uint16, key [rounds]uint16) [2]uint16 {

	l := data[0]
	r := data[1]

	for i := 0; i < rounds; i++ {
		tmp := l ^ roundFunc(r, key[i])
		l = r
		r = tmp
	}
	return [2]uint16{l, r}
}

func decrypt(data [2]uint16, key [rounds]uint16) [2]uint16 {

	l := data[0]
	r := data[1]

	for i := rounds - 1; i >= 0; i-- {
		tmp := r ^ roundFunc(l, key[i])
		r = l
		l = tmp
	}
	return [2]uint16{l, r}
}

func roundFunc(right uint16, key uint16) uint16 {
	right += key
	return right
}
