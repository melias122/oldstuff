package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"strconv"
)

type sliceMatrix struct {
	n, m int
	data [][]uint8
}

func NewSliceMatrix(n, m int) Matrix {
	mat := sliceMatrix{
		n:    n,
		m:    m,
		data: make([][]uint8, n),
	}
	for i := range mat.data {
		mat.data[i] = make([]uint8, m)
	}
	return mat
}

func (s sliceMatrix) Size() (int, int) {
	return s.n, s.m
}

func (s sliceMatrix) Insert(i, j int, element uint8) {
	e := s.data[i][j]
	s.data[i][j] = (element + e) % 2
}

func (s sliceMatrix) Element(i, j int) uint8 {
	return s.data[i][j]
}

func (s sliceMatrix) String() string {
	var buf bytes.Buffer
	for _, row := range s.data {
		var sep string
		for _, el := range row {
			buf.WriteString(sep)
			buf.WriteString(strconv.FormatUint(uint64(el), 10))
			sep = " "
		}
		buf.WriteString("\n")
	}
	return buf.String()
}

func main() {
	f, err := os.Open("file.txt")
	if err != nil {
		log.Fatal(err)
	}
	mat, err := readMatrix(f)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(mat)
	Gauss(mat)
	fmt.Println(mat)
}
