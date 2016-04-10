package main

import "fmt"

type Node struct {
	value rune
	nodes []*Node
}

func NewNode(value rune) *Node {
	return &Node{value: value}
}

func (n *Node) String() string {
	return fmt.Sprint(string(n.value))
}

type BST struct {
	root *Node
}

func (b *BST) String() string {
	return b.levelOrder([]*Node{b.root})
}

func (b *BST) levelOrder(nodes []*Node) string {
	var (
		s   string
		sep string
	)
	for _, node := range nodes {
		if len(node.nodes) > 0 {
			s += b.levelOrder(node.nodes)
		}
		s += fmt.Sprintf("%s%s", sep, string(node.value))
	}
	return s
}

func (b *BST) Put(s string) {
	if b.root == nil {
		b.root = NewNode(0)
	}
	current := b.root
	for _, value := range s {
		var next *Node
		for _, node := range current.nodes {
			if node.value == value {
				next = node
				break
			}
		}
		if next == nil {
			next = NewNode(value)
			current.nodes = append(current.nodes, next)
		}
		current = next
	}
}

func main() {
	b := &BST{root: NewNode(0)}
	b.Put("jano")
	b.Put("jazo")
	b.Put("fero")
	b.Put("karol")
	fmt.Println(b)
}
