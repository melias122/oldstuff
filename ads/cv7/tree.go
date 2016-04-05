package main

import (
	"bytes"
	"fmt"
	"math/rand"
)

type Node struct {
	value       int
	left, right *Node
}

func NewNode(value int) *Node {
	return &Node{value: value}
}

func (n *Node) String() string {
	return fmt.Sprint(n.value)
}

type BST struct {
	root *Node
}

func (b *BST) String() string {
	return b.levelOrder(b.root)
}

func (b *BST) levelOrder(node *Node) string {
	var (
		fun func(nodes []*Node)
		buf bytes.Buffer
	)
	fun = func(nodes []*Node) {
		next := []*Node{}
		var sep string
		for _, n := range nodes {
			buf.WriteString(sep + n.String())
			sep = " "
			if n.right != nil {
				next = append(next, n.right)
			}
			if n.left != nil {
				next = append(next, n.left)
			}
		}
		if len(next) > 0 {
			buf.WriteString("\n")
			fun(next)
		}
	}
	fun([]*Node{node})
	return buf.String()
}

func (b *BST) inorderString(node *Node) string {
	var s string
	if node.left != nil {
		s += b.inorderString(node.left)
	}
	s += node.String()
	if node.right != nil {
		s += b.inorderString(node.right)
	}
	return s
}

func (b *BST) Put(value int) {
	b.put(value, &b.root)
}

func (b *BST) put(value int, node **Node) {
	if *node == nil {
		*node = NewNode(value)
		return
	}
	if (*node).value == value {
		return
	} else if value < (*node).value {
		b.put(value, &(*node).left)
	} else {
		b.put(value, &(*node).right)
	}
}

func main() {
	b := &BST{}
	for i := 0; i < 10; i++ {
		b.Put(rand.Int() % 10)
	}
	fmt.Println(b)
}
