package main

import (
	"fmt"
	"testing"
)

func printH(h *hashTable) {
	fmt.Println("size: ", h.count, "cap: ", len(h.buckets))
	for _, b := range h.buckets {
		fmt.Print("[")
		for b != nil {
			fmt.Printf("%v: %v ", b.key, b.value)
			b = b.next
		}
		fmt.Println("]")
	}
}

func TestHashTable(t *testing.T) {
	h := NewHashTableSize(8)
	h.Put("Martin", 1)
	fmt.Println(h.Get("Martin"))
	fmt.Println(h.Get(""))
	printH(h)
}
