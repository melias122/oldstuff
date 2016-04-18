package main

import (
	"fmt"
	"math/rand"
	"testing"
)

func randomString(max int) string {
	b := make([]byte, rand.Int()%max+1)
	for i := range b {
		b[i] = byte(rand.Int())
	}
	return string(b)
}

func printH(h *hashTable) {
	var empty int
	for _, b := range h.buckets {
		if b == nil {
			empty++
		}
		fmt.Print("[")
		for b != nil {
			fmt.Printf("%v: %v ", b.key, b.value)
			b = b.next
		}
		fmt.Println("]")
	}
	fmt.Println("size: ", h.count, "cap: ", len(h.buckets))
	fmt.Println("empty buckets: ", empty)
}

func TestHashTableRandom(t *testing.T) {
	h := NewHashTableSize(1000)
	for i := 0; i < len(h.buckets); i++ {
		s := randomString(50)
		h.Put(s, len(s))
	}
	printH(h)
}
