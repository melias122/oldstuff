package main

import (
	"fmt"
	"testing"
)

func printTrie(t *trie) {
	m := make(map[int]string)
	levelOrder(0, m, []*trieNode{t.root})
	for i := 1; ; i++ {
		if v, ok := m[i]; ok {
			fmt.Printf("%d:%s\n", i, v)
		} else {
			break
		}
	}
}

func levelOrder(i int, m map[int]string, n []*trieNode) {
	for _, node := range n {
		m[i] += string(node.value)
		if len(node.nodes) > 0 {
			levelOrder(i+1, m, node.nodes)
		}
	}
}

func TestTrie(t *testing.T) {
	trie := NewTrie()

	tests := []string{
		"Martin",
		"Maros",
		"Jozo",
		"Fero",
		"oref",
	}

	for _, s := range tests {
		trie.Put(s)
	}
	for _, s := range tests {
		if !trie.Contains(s) {
			t.Fatalf("Expected %s to be in trie", s)
		}
	}
	for _, s := range []string{"Martis", "martin"} {
		if trie.Contains(s) {
			t.Fatalf("Expected %s to be not in trie", s)
		}
	}
}
