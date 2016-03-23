package main

import (
	"math/big"
	"testing"
)

func TestLFSR_MaxPeriod(t *testing.T) {
	d := 16
	lfsr, err := NewLFSR(d)
	if err != nil {
		panic(err)
	}

	state0 := new(big.Int).Set(lfsr.state)
	count := big.NewInt(0)
	for {
		count.Add(count, bigOne)

		r := lfsr.RandInt()
		// fmt.Println(r)
		if r.Cmp(state0) == 0 {
			break
		}

	}
	// maximalna perioda je 2^d - 1
	maxperiod := new(big.Int).Lsh(bigOne, uint(d))
	maxperiod.Sub(maxperiod, bigOne)
	if count.Cmp(maxperiod) != 0 {
		t.Fatal("Nebola dosiahnuta maximalna perioda")
	}
}

func TestLFSR_NextPrime(t *testing.T) {
	d := 8
	lfsr, _ := NewLFSR(d)
	for i := 0; i < 100; i++ {
		lfsr.NextPrime()
		// fmt.Println(p)
	}
}

func TestLFSR_NextPrimeBits(t *testing.T) {
	d := 512
	lfsr, _ := NewLFSR(d)
	for i := 0; i < 10; i++ {
		lfsr.NextPrimeBits(256)
	}
}
