package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"
)

func read(path string) ([][]int, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var rows, cols int
	if _, err := fmt.Fscanf(file, "%d\n%d\n", &rows, &cols); err != nil {
		return nil, err
	}

	arr := make([][]int, rows)
	for i := range arr {
		arr[i] = make([]int, cols)
		if _, err := fmt.Fscanf(file, "%d %d %d\n", &arr[i][0], &arr[i][1], &arr[i][2]); err != nil {
			return nil, err
		}
	}
	return arr, nil
}

func abs(i int) int {
	if i < 0 {
		return -i
	}
	return i
}

func closer(ints []int, sum int) int {
	var c int
	for _, i := range ints {
		if abs(abs(sum)-i) <= sum {
			c = -i
			break
		}
	}
	if c == 0 {
		c = ints[rand.Int()%len(ints)]
	}
	return c
}

func sum(ints [][]int) (int, []int) {
	var sum int
	var ints2 []int
	for _, i := range ints {
		c := closer(i, sum)
		sum += c
		ints2 = append(ints2, c)
	}
	return sum, ints2
}

func main() {
	rand.Seed(time.Now().Unix())
	ints, err := read("sum.txt")

	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	for {
		sum, vals := sum(ints)
		if abs(sum) != 0 {
			continue
		}
		var test int
		for _, v := range vals {
			test += v
			fmt.Println(v)
		}
		fmt.Println(test)
		break
	}
}
