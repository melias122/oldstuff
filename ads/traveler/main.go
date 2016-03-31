package main

import (
	"fmt"
	"io"
	"log"
	"math"
	"os"
)

func read(r io.Reader) ([][]int, error) {
	var n int
	if _, err := fmt.Fscanln(r, &n); err != nil {
		return nil, err
	}
	mat := make([][]int, n)
	for i := range mat {
		mat[i] = make([]int, n)
		for j := range mat[i] {
			if _, err := fmt.Fscan(r, &mat[i][j]); err != nil {
				return nil, err
			}
		}
	}
	return mat, nil
}

func Dynamic(w io.Writer, mat [][]int) {

}

func Greedy(w io.Writer, mat [][]int) {
	var (
		distance int
		current  int
		visited  = make(map[int]bool)
	)
	for len(visited) < len(mat) {
		var (
			min  = math.MaxInt64
			next int
		)
		// mark current as visited
		visited[current] = true

		// itterate over all citties
		// and find minimal distance
		for i := range mat[current] {
			// skip visited
			if visited[i] {
				continue
			}
			if mat[i][current] < min {
				min = mat[current][i]
				next = i
			}
		}
		// print current to output
		fmt.Fprintln(w, current+1)
		distance += mat[current][next]
		current = next
	}
	// print last(first)
	fmt.Fprintln(w, current+1)
	// fmt.Println("Distance:", distance)
}

func main() {
	f, err := os.Open("vstup.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	// read matrix
	mat, err := read(f)

	// create file for A
	a, err := os.Create("A.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer a.Close()

	// dynamic solver
	Dynamic(a, mat)

	// create file for B
	b, err := os.Create("B.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer b.Close()

	// greedy solver
	Greedy(b, mat)
}
