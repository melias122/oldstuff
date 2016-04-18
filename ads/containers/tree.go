package main

import "sort"

type obvsNode struct {
	left, right *obvsNode
	key         string
}

func newObvsNode(words []word, R [][]int, i, j int) *obvsNode {
	if i != j {
		return &obvsNode{
			key:   words[R[i][j]].str,
			left:  newObvsNode(words, R, i, R[i][j]-1),
			right: newObvsNode(words, R, R[i][j], j),
		}
	}
	return nil
}

type obvs struct {
	root *obvsNode
}

func (o *obvs) Contains(key string) bool {
	node := o.root
	for node != nil {
		if key == node.key {
			return true
		} else if key < node.key {
			node = node.left
		} else {
			node = node.right
		}
	}
	return false
}

func NewOBVS(words []word, dictionary []word) *obvs {

	sort.Sort(wordSorter(words))
	sort.Sort(wordSorter(dictionary))

	var (
		p = make([]int, len(words)+1)
		q = make([]int, len(words)+1)
		n = len(p)
		W = new2DInts(n)
		C = new2DInts(n)
		R = new2DInts(n)
	)

	for i, w := range words {
		p[i+1] = w.p
	}

	// find q values
	var i int
	for j, w := range words {
		for dictionary[i] != w {
			q[j] += dictionary[i].p
			i++
		}
		i++
	}

	// create W matrix
	for i := range q {
		W[i][i] = q[i]
		for j := i + 1; j < n; j++ {
			W[i][j] = W[i][j-1] + p[j] + q[j]
		}
	}

	// init C matrix
	for i := 0; i < n; i++ {
		C[i][i] = W[i][i]
	}

	// create C, init R
	for i := 0; i < n-1; i++ {
		j := i + 1
		C[i][j] = C[i][i] + C[j][j] + W[i][j]
		R[i][j] = j
	}

	// find R matrix
	for i := 2; i < n; i++ {
		for j := 0; j < n-i; j++ {
			var (
				k   = j + i
				m   = R[j][k-1]
				min = C[j][m-1] + C[m][k]
			)

			// find minimum
			for l := m + 1; l <= R[j+1][k]; l++ {
				x := C[j][l-1] + C[l][k]
				if x < min {
					m = l
					min = x
				}
			}
			C[j][k] = W[j][k] + min
			R[j][k] = m
		}
	}

	// we need first word empty
	_words := append([]word{{}}, words...)
	return &obvs{
		root: newObvsNode(_words, R, 0, len(_words)-1),
	}
}

func new2DInts(n int) [][]int {
	ints := make([][]int, n)
	for i := range ints {
		ints[i] = make([]int, n)
	}
	return ints
}
