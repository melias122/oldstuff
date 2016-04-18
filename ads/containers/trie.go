package main

type trieNode struct {
	key   rune
	value string
	nodes []*trieNode
}

func newTrieNode(key rune, value string) *trieNode {
	return &trieNode{key: key, value: value}
}

type trie struct {
	root *trieNode
}

func NewTrie() *trie {
	return &trie{}
}

func (t *trie) Put(value string) {
	if t.root == nil {
		t.root = newTrieNode(0, "")
	}
	node := t.root
	for i, v := range value {
		var next *trieNode
		for _, node := range node.nodes {
			if node.key == v {
				next = node
				break
			}
		}
		if next == nil {
			var j int
			if i+1 == len(value) {
				j = i + 1
			}
			next = newTrieNode(v, value[:j])
			node.nodes = append(node.nodes, next)
		}
		node = next
	}
}

func (t *trie) Contains(value string) bool {
	if t.root == nil {
		return false
	}
	node := t.root
	for _, v := range value {
		var next *trieNode
		for _, node := range node.nodes {
			if node.key == v {
				next = node
				break
			}
		}
		if next == nil {
			return false
		}
		node = next
	}
	if node.value != value {
		return false
	}
	return true
}
