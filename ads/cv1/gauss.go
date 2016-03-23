package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"strconv"
)

type Typ int

const (
	Slice Typ = iota
	Bytes
)

type Matrix interface {
	Size() (int, int)
	Insert(i, j int, element uint8)
	Element(i, j int) uint8
	String() string
}

func readMatrix(r io.Reader, typ Typ) (Matrix, error) {

	in := csv.NewReader(r)
	in.Comma = ' '
	size, err := in.Read()
	if err != nil {
		return nil, err
	}

	if len(size) != 2 {
		return nil, fmt.Errorf("First row needs size 'nrows ncolums'")
	}

	n, err := strconv.Atoi(size[0])
	if err != nil {
		return nil, err
	}

	m, err := strconv.Atoi(size[1])
	if err != nil {
		return nil, err
	}

	var mat Matrix
	switch typ {
	case Slice:
		mat = NewSliceMatrix(n, m)
	default:
		panic("undefined matrix type")
	}

	in.FieldsPerRecord = m
	matrix, err := in.ReadAll()
	if err != nil {
		return nil, err
	}
	for i, row := range matrix {
		for j, el := range row {
			element, err := strconv.ParseUint(el, 10, 8)
			if err != nil {
				return nil, err
			}
			mat.Insert(i, j, uint8(element))
		}
	}
	return mat, nil
}

func Gauss(mat Matrix) {
	n, m := mat.Size()
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			if mat.Element(j, i) == 0 {
				continue
			}
			for k := i; k < m; k++ {
				mat.Insert(j, k, mat.Element(i, k))
			}
		}
	}
}
