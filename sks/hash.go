package main

var poly32 = uint32(0x80000057)

func hash32(in []byte) []byte {
	// initial value
	h := uint32(0x7)
	for _, b := range in {
		h ^= uint32(b)
		h *= poly32
		h ^= h >> 15
	}
	return []byte{byte(h >> 24), byte(h >> 16), byte(h >> 8), byte(h)}
}
