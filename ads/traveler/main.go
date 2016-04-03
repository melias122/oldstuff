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

type Solution struct {
	// track    bytes.Buffer
	track    []byte
	distance int
}

func NewSolution(d int, t ...byte) *Solution {
	return &Solution{
		track:    append(make([]byte, 0, 32), t...),
		distance: d,
	}
}

func (s *Solution) String() string {
	return string(s.track)
}

func (d *Distance) Find(from int, G []byte, min int) *Solution {
	if len(G) == 1 {
		return NewSolution(
			d.mat[from][G[0]]+d.mat[G[0]][0],
			[]byte{1, G[0] + 1, byte(from) + 1}...,
		)
	}

	var (
		best *Solution
	)
	for i, next := range G {

		C := d.mat[from][next]

		nextG := append(make([]byte, 0, len(G)-1), G[:i]...)
		nextG = append(nextG, G[i+1:]...)

		solution, ok := d.get(int(next), nextG)
		if !ok {
			solution = d.Find(int(next), nextG, min)
			if solution == nil {
				continue
			}
			d.add(solution)
		}

		if solution.distance+C < min {
			s := NewSolution(solution.distance+C, solution.track...)
			s.track = append(s.track, byte(from)+1)
			min = s.distance
			best = s
		}
	}
	if len(G) > 12 && best != nil {
		fmt.Println(len(G), best.track)
	}
	return best
}

func (d *Distance) get(from int, g []byte) (s *Solution, ok bool) {
	buf := []byte{1}
	buf = append(buf, g...)
	s, ok = d.solutions[string(buf)]
	return
}

func (d *Distance) add(s *Solution) {
	d.solutions[s.String()] = s
}

type Distance struct {
	mat [][]int
	min int

	solutions map[string]*Solution
}

func Dynamic(w io.Writer, mat [][]int) {
	solver := Distance{
		mat:       mat,
		solutions: make(map[string]*Solution),
	}
	to := make([]uint8, len(mat)-1)
	for i := range to {
		to[i] = uint8(i + 1)
	}
	s := solver.Find(0, to, math.MaxInt64)
	fmt.Println(s.distance, s.track)
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

	// d := Distance{
	// 	solutions: make(map[string]*Solution),
	// }
	// g := []byte{1, 2, 3, 1}
	// s := NewSolution(0, g...)
	// fmt.Println(s.track)
	// d.add(s)
	// s2, ok := d.get(1, g[1:])
	// fmt.Println(s2, ok)
}
