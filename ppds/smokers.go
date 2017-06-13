package main

import (
	"fmt"
	"sync"
	"time"
	"math/rand"
)

var (
	t = make(chan int)
	p = make(chan int)
	m = make(chan int)
)

func agent() {
	for {
		time.Sleep(time.Second)
		select {
		case t <- 1:
		case p <- 1:
		case m <- 1:
		}
	}
}

func pusher() {
	
}

func main() {
	
	agent()
}
