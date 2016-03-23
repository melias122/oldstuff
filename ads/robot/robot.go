package main

import (
	"fmt"
	"io"
	"log"
	"math"
	"math/rand"
	"os"
	"time"
)

type Robot struct {
	Object

	moves     int
	collected int
}

func (r Robot) String() string {
	if r.moves > 0 {
		return fmt.Sprintf("Collected %d objects in %d moves", r.collected, r.moves)
	}
	return fmt.Sprintf("Anything to collect?\nYou need to give me some objects.")
}

func (r *Robot) Collect(objects []Object, strategy Strategy) {
	strategy(objects)
	for _, o := range objects {
		r.moves += r.distance(o)
		r.Object = o
		r.collected++
	}
}

type Object struct {
	x, y int
}

func (a Object) distance(b Object) int {
	x := a.x - b.x
	if x < 0 {
		x = -x
	}
	y := a.y - b.y
	if y < 0 {
		y = -y
	}
	return y + x
}

func ReadObjects(r io.Reader) ([]Object, error) {
	var objects []Object
	for {
		var o Object
		_, err := fmt.Fscanf(r, "%d %d\n", &o.x, &o.y)
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}
		objects = append(objects, o)
	}
	return objects, nil
}

func nearest(last Object, objects []Object) ([]Object, []int) {
	var (
		winners []Object
		indices []int
		min     = int(math.MaxInt64)
	)
	for i, o := range objects {
		d0 := last.distance(o)
		if d0 < min {
			min = d0
			indices = indices[:0]
			winners = winners[:0]
		}
		if d0 == min {
			min = d0
			indices = append(indices, i)
			winners = append(winners, o)
		}
	}
	return winners, indices
}

type Strategy func([]Object)

func swapObjects(objects []Object, i, j int) {
	objects[i], objects[j] = objects[j], objects[i]
}

func RandomChooseNearest(objects []Object) {
	var last Object
	for k := 0; k < len(objects); k++ {

		winners, indices := nearest(last, objects[k:])

		// random choose nearest
		i := rand.Int() % len(indices)
		swapObjects(objects, k, indices[i]+k)
		last = winners[i]
	}
}

func NearestFirst(objects []Object) {
	var last Object
	for k := 0; k < len(objects); k++ {

		winners, indices := nearest(last, objects[k:])

		// take first
		swapObjects(objects, k, indices[0]+k)
		last = winners[0]
	}
}

func init() {
	rand.Seed(time.Now().Unix())
}

func main() {
	var (
		s Strategy
		r Robot
	)

	// may give best overall results
	// gives around ~59000  to ~62000 moves
	// s = RandomChooseNearest

	// best in this case.
	// 60161 moves in less than 0.100s
	s = NearestFirst

	// read objects from stdin
	objects, err := ReadObjects(os.Stdin)
	if err != nil {
		log.Fatal(err)
	}

	r.Collect(objects, s)
	fmt.Println(r)
}
