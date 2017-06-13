package main

import(
	"fmt"
	"time"
	"math/rand"
	"sync"
)

type forks struct {
	sema chan int
	fork []sync.Mutex
}

func NewForks(n int) *forks {	
	return &forks{
		sema: make(chan int, n-1),
		fork: make([]sync.Mutex, n),
	}
}

func (f *forks) Get(i int) {
	f.sema <- 1
	f.fork[i].Lock()
	f.fork[(i+1)%len(f.fork)].Lock()
}

func (f *forks) Put(i int) {
	<-f.sema
	f.fork[i].Unlock()
	f.fork[(i+1)%len(f.fork)].Unlock()
}

type philosopher struct {
	id int
	forks *forks
}

func NewPhilosopher(id int, forks *forks) *philosopher {
	return &philosopher{
		id: id,
		forks: forks,
	}
}

func (p *philosopher) think() {
	fmt.Printf("%d: thinking\n", p.id)
	time.Sleep(time.Duration(rand.Intn(2)) * time.Second)
}

func (p *philosopher) eat() {
	fmt.Printf("%d: eating\n", p.id)
	time.Sleep(time.Duration(rand.Intn(3)) * time.Second)
}

func (p *philosopher) run() {
	for {
		p.think()
		p.forks.Get(p.id)
		p.eat()
		p.forks.Put(p.id)
	}
}

func main(){
	i := 0
	n := 5
	
	forks := NewForks(n)
	for ; i < n-1; i++ {
		p := NewPhilosopher(i, forks)
		go p.run()
	}
	p := NewPhilosopher(i, forks)
	p.run()
}
