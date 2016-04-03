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

type c struct {
	k int
	b string
}

func toC(to int, via []byte) c {
	return c{k: to, b: string(via)}
}

func fromC(c c) (int, []byte) {
	return c.k, []byte(c.b)
}

func delete(b []byte, i int) []byte {
	new := make([]byte, 0, len(b)-1)
	new = append(new, b[:i]...)
	return append(new, b[i+1:]...)
}

type DynamicTSP struct {
	cost  [][]int
	cache map[c]c
}

func NewDynamicTSP(mat [][]int) *DynamicTSP {
	d := &DynamicTSP{
		cost:  mat,
		cache: make(map[c]c),
	}
	return d
}

func (d *DynamicTSP) Solve() (int, []byte) {
	via := make([]byte, len(d.cost)-1)
	for i := range via {
		via[i] = byte(i + 1)
	}
	for i := 1; i < len(d.cost); i++ {
		d.cache[toC(i, []byte{})] = toC(d.cost[i][0], []byte{0, byte(i)})
	}
	return d.MinCost(0, via)
}

func (d *DynamicTSP) Cost(to byte, via []byte) (int, []byte) {
	key := toC(int(to), via)
	c, ok := d.cache[key]
	if ok {
		i, b := fromC(c)
		return i, b
	}
	distance, track := d.MinCost(to, via)
	d.cache[key] = toC(distance, track)
	return distance, track
}

func (d *DynamicTSP) MinCost(to byte, via []byte) (int, []byte) {
	var (
		track   []byte
		minCost = math.MaxInt32
		nexto   byte
	)
	for i, next := range via {
		cost, subtrack := d.Cost(next, delete(via, i))
		if cost < minCost {
			minCost = cost
			nexto = next
			track = append(subtrack, to)
		}
	}
	return minCost + d.cost[to][nexto], track
}

func (d *DynamicTSP) Write(track []byte) {
	// create file for A
	a, err := os.Create("A.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer a.Close()
	for i := len(track) - 1; i >= 0; i-- {
		fmt.Fprintln(a, track[i]+1)
	}
}

func Greedy(w io.Writer, mat [][]int) {
	var (
		distance int
		current  int
		visited  = make(map[int]bool)
		track    []int
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
		track = append(track, current+1)
		distance += mat[current][next]
		current = next
	}
	track = append(track, current+1)
	for _, t := range track {
		fmt.Fprintln(w, t)
	}
	// fmt.Println(distance, track)
}

func resize(mat [][]int, i int) [][]int {
	new := make([][]int, i)
	for r := range new {
		new[r] = make([]int, i)
		for s := range new[r] {
			new[r][s] = mat[r][s]
		}
	}
	return new
}

func main() {
	f, err := os.Open("vstup.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	// read matrix
	mat, err := read(f)
	if err != nil {
		log.Fatal(err)
	}

	// create file for B
	b, err := os.Create("B.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer b.Close()

	// greedy solver
	Greedy(b, mat)

	// dynamic solver
	// try more if you have more than 4gb ram
	mat = resize(mat, 21)
	tsp := NewDynamicTSP(mat)
	_, track := tsp.Solve()
	tsp.Write(track)
}
