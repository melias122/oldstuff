package main

import (
	"fmt"
)

func main() {

	comb(3, 5, func(c []int) {
		fmt.Println(c)
	})
}

func comb(n, m int, emit func([]int)) {
	s := make([]int, n)
	last := n - 1

	var rc func(int, int)

	rc = func(i, next int) {
		for j := next; j < m; j++ {
			s[i] = j
			// s[i] = j + 1
			if i == last {
				emit(s)
			} else {
				rc(i+1, j+1)
			}
		}
	}
	rc(0, 0)
}
