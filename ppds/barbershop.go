package main

import (
	"fmt"
	"sync"
	"time"
)

type shop struct {
	mu sync.Mutex
	customers, n int
	customer, barber, customerDone, barberDone chan int
}

func newShop(n int) *shop{
	return &shop{
		n: n,
		customer: make(chan int, 1),
		customerDone: make(chan int, 1),
		barber: make(chan int, 1),
		barberDone: make(chan int, 1),
	}
}

var balkChan = make(chan int)

func balk() {
	<-balkChan
	fmt.Println("...")
}

func customer(s *shop) {
	s.mu.Lock()
	if s.customers == s.n {
		s.mu.Unlock()
		balk()
	}
	s.customers++
	s.mu.Unlock()

	s.customer <- 1
	<-s.barber

	fmt.Println("getting haircut")

	s.customerDone <- 1
	<-s.barberDone

	s.mu.Lock()
	s.customers--
	s.mu.Unlock()

}

func barber(s *shop) {
	for {
		<-s.customer
		s.barber <- 1

		fmt.Println("haircut")
		time.Sleep(time.Second)

		<-s.customerDone
		s.barberDone <- 1
	}
}

func main() {
	s := newShop(4)

	go barber(s)

	for i:=0;i<5;i++ {
		go customer(s)
	}
	
	time.Sleep(10 * time.Second)
}
