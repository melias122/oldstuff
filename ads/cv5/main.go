package main

import (
	"fmt"
	"math/big"
)

// O(2^n)
func Fib(n int) int {
	if n == 0 || n == 1 {
		return 1
	}
	return Fib(n-1) + Fib(n-2)
}

var fibtab = map[int]int{0: 1, 1: 1}

// ~O(n)
func Fib2(n int) int {
	f, ok := fibtab[n]
	if !ok {
		f = Fib2(n-1) + Fib2(n-2)
		fibtab[n] = f
	}
	return f
}

// O(n)
func Fib3(n int) *big.Int {
	f0, f1 := big.NewInt(1), big.NewInt(1)
	for i := 0; i < n; i++ {
		tmp := new(big.Int).Set(f1)
		f1.Add(f0, f1)
		f0.Set(tmp)
	}
	return f0
}

func lcs(S, T string, p, q int, tab [][]int) int {
	v := 0
	if S[p] == T[q] {
		v = lcs(S, T, p-1, q-1, tab) + 1
	}
	tab[p][q] = v
	return v
}

func LCS(S, T string) int {
	tab := make([][]int, len(S))
	for i := range tab {
		tab[i] = make([]int, len(T))
	}
	lcs(S, T, len(S)-1, len(T)-1, tab)
	fmt.Println(tab)
	return 0
}

func main() {
	// n := 5000
	// fmt.Println(Fib(n))
	// fmt.Println(Fib2(n))
	// fmt.Println(Fib3(5000))

	S := "000111010101010100011000110001111100"
	T := "01001010100000110000111111000100101010"
	LCS(S, T)
}
