package main

import (
	"fmt"
	"testing"
)

func printObvs(node *obvsNode, nivel int) {
	if node != nil {
		printObvs(node.right, nivel+1)
		for i := 0; i <= nivel; i++ {
			fmt.Printf("\t")
		}
		fmt.Printf("%s\n", node.key)
		printObvs(node.left, nivel+1)
	}
}

func TestTree(t *testing.T) {
	words, dictionary := createInput("slovnik.txt")
	o := NewOBVS(words, dictionary)
	for _, w := range words {
		if !o.Contains(w.str) {
			t.Fatal(w.str, " expected to be in obvs")
		}
	}
	// printObvs(o.root, 0)
}
