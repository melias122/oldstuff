package main

import "fmt"

type trieNode struct {
	key   string
	value rune
	nodes []*trieNode
}

func newTrieNode(value rune) *trieNode {
	return &trieNode{value: value}
}

func (n *trieNode) String() string {
	return fmt.Sprint(string(n.value))
}

type trie struct {
	root *trieNode
}

func NewTrie() *trie {
	return &trie{}
}

func (t *trie) Put(value string) {
	if t.root == nil {
		t.root = newTrieNode(0)
	}
	node := t.root
	for _, v := range value {
		var next *trieNode
		for _, node := range node.nodes {
			if node.value == v {
				next = node
				break
			}
		}
		if next == nil {
			next = newTrieNode(v)
			node.nodes = append(node.nodes, next)
		}
		node = next
	}
}

func (t *trie) Get(value string) {

}
