package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
)

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func Lcs(X, Y string) int {
	if len(X) <= 1 || len(Y) <= 1 {
		return 0
	}
	C := make([][]int, len(X))
	for i := range C {
		C[i] = make([]int, len(Y))
	}
	for i := 1; i < len(X); i++ {
		for j := 1; j < len(Y); j++ {
			if X[i] == Y[j] {
				C[i][j] = C[i-1][j-1] + 1
			} else {
				C[i][j] = max(C[i][j-1], C[i-1][j])
			}
		}
	}
	return C[len(X)-1][len(Y)-1]
}

func Edit(X, Y string) int {
	if len(X) <= 1 || len(Y) <= 1 {
		return 0
	}
	C := make([][]int, len(X))
	for i := range C {
		C[i] = make([]int, len(Y))
	}
	for i := 1; i < len(X); i++ {
		var k int
		for j := 1; j < len(Y); j++ {
			if X[i] != Y[j] {
				k = 1
			} else {
				k = 0
			}
			m := min(C[i-1][j]+1, C[i][j-1]+1)
			C[i][j] = min(C[i-1][j-1]+k, m)
		}
	}
	return C[len(X)-1][len(Y)-1]
}

func main() {
	if len(os.Args) != 3 {
		fmt.Fprintf(os.Stderr, "Usage\n%s file1 file2\n", os.Args[0])
		os.Exit(1)
	}
	X, err := ioutil.ReadFile(os.Args[1])
	if err != nil {
		log.Fatal(err)
	}
	Y, err := ioutil.ReadFile(os.Args[2])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(Edit(string(X), string(Y)))
}
