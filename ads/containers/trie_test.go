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
	trie.Put("Martin")
	trie.Put("Maros")
	trie.Put("Jozo")
	printTrie(trie)
	// t.Log(trie.String())
}
