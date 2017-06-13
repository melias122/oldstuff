package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

type pot struct {
	mu          sync.Mutex
	servings    int
	empty, full chan struct{}
}

func savage(n int, p *pot) {
	for {
		// get serving from pot
		p.mu.Lock()
		if p.servings == 0 {
			p.empty <- struct{}{}
			<-p.full
		}
		p.servings--
		p.mu.Unlock()

		// eat
		fmt.Printf("%d: eating nom, nom ..\n", n)
		time.Sleep(2 * time.Second)
	}
}

func cook(p *pot) {
	for {
		// put serving in pot
		<-p.empty
		p.servings = 1 + rand.Int()%10
		fmt.Printf("puting %d in pot\n", p.servings)
		p.full <- struct{}{}
	}
}

func main() {
	var (
		n = 10
		p = &pot{
			empty: make(chan struct{}, 1),
			full: make(chan struct{}, 1),
		}
	)
	for i := 0; i < n; i++ {
		go savage(i, p)
	}

	cook(p)
}
