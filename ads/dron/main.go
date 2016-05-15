package main

import (
	"bufio"
	"fmt"
	"log"
	"math"
	"os"
)

const nodata = -9999

type point struct {
	x, y int
}

func (p point) String() string {
	return fmt.Sprintf("%d %d", p.x, p.y)
}

func (p point) ok() bool {
	return p.x >= 0 && p.x < 6001 && p.y >= 0 && p.y < 6001
}

func (p point) up() (point, bool) {
	new := point{p.x, p.y + 1}
	return new, new.ok()
}

func (p point) right() (point, bool) {
	new := point{p.x + 1, p.y}
	return new, new.ok()
}

func (p point) down() (point, bool) {
	new := point{p.x, p.y - 1}
	return new, new.ok()
}

func (p point) left() (point, bool) {
	new := point{p.x - 1, p.y}
	return new, new.ok()
}

type dron struct {
	point
	visited map[point]bool
	heigth  int
}

// cost a -> b
func cost(a, b point, x [][]int) float64 {
	var c float64
	ha := float64(x[a.x][a.y])
	hb := float64(x[b.x][b.y])

	if hb > 1200 {
		return math.MaxFloat64
	}

	if hb > ha {
		c = (hb - ha) * 1.2
	}
	return 1 + c
}

func min(a, b float64) float64 {
	if a < b {
		return a
	}
	return b
}

func find(x [][]int, nodata int) {
	var (
		start     = point{0, 6000}
		end       = point{6000, 0}
		dron      = dron{start, make(map[point]bool), 0}
		trackCost float64
		next      point
	)

	for dron.point != end {
		fmt.Println(dron.point)

		min := math.MaxFloat64

		u, ok := dron.up()
		if ok && !dron.visited[u] {
			c := cost(dron.point, u, x)
			if c < min {
				min = c
				next = u
			}
		}

		r, ok := dron.right()
		if ok && !dron.visited[r] {
			c := cost(dron.point, r, x)
			if c < min {
				min = c
				next = r
			}
		}

		d, ok := dron.down()
		if ok && !dron.visited[d] {
			c := cost(dron.point, d, x)
			if c < min {
				min = c
				next = d
			}
		}

		l, ok := dron.left()
		if ok && !dron.visited[l] {
			c := cost(dron.point, l, x)
			if c < min {
				min = c
				next = l
			}
		}

		dron.visited[dron.point] = true
		dron.point = next
		trackCost += min
	}
	fmt.Println(dron.point)
}

func main() {
	file, err := os.Open("srtm_40_03.asc")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	r := bufio.NewReaderSize(file, 4096*1000)

	var (
		discardf             float64
		discard              string
		ncols, nrows, nodata int
	)

	fmt.Fscanln(r, &discard, &ncols)
	fmt.Fscanln(r, &discard, &nrows)
	fmt.Fscanln(r, &discard, &discardf)
	fmt.Fscanln(r, &discard, &discardf)
	fmt.Fscanln(r, &discard, &nodata)

	x := make([][]int, nrows)
	for i := range x {
		x[i] = make([]int, ncols)
		for j := range x[i] {
			fmt.Fscan(r, &x[i][j])
		}
	}
	find(x, nodata)
}
