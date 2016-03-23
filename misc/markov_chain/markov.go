package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"time"
)

const (
	NPREF  = 2
	NHASH  = 4093
	MAXGEN = 10000

	NOWORD = "\n"
)

type Prefix [NPREF]string

func (p *Prefix) add(word string) {
	for i, w := range p[1:] {
		p[i] = w
	}
	p[len(p)-1] = word
}

type Suffix struct {
	suf []string
}

func newSuffix() *Suffix {
	return &Suffix{
		suf: []string{},
	}
}

func (s *Suffix) add(word string) {
	s.suf = append(s.suf, word)
}

func (s *Suffix) randWord() string {
	return s.suf[rand.Int()%len(s.suf)]
}

type Chain struct {
	pref     Prefix
	stateTab map[Prefix]*Suffix
}

func newChain() Chain {
	return Chain{
		stateTab: make(map[Prefix]*Suffix),
	}
}

func (c *Chain) build(file *os.File) error {

	sc := bufio.NewScanner(file)
	sc.Split(bufio.ScanWords)

	for sc.Scan() {
		word := strings.TrimRight(sc.Text(), ",.; :")
		_, err := strconv.Atoi(word)
		if err == nil {
			continue
		}
		c.add(word)
	}
	c.add(NOWORD)

	return sc.Err()
}

func (c *Chain) add(word string) {
	var (
		ok     bool
		suffix *Suffix
	)
	if suffix, ok = c.stateTab[c.pref]; !ok {
		suffix = newSuffix()
		c.stateTab[c.pref] = suffix
	}
	suffix.add(word)
	c.pref.add(word)
}

func (c *Chain) generate(n int) {
	var prefix Prefix
	for i := 0; i < n; i++ {
		suffix := c.stateTab[prefix]
		word := suffix.randWord()
		if word == NOWORD {
			break
		}
		fmt.Print(word, " ")
		prefix.add(word)
	}
	fmt.Println("")
}

func main() {
	rand.Seed(time.Now().UTC().UnixNano())
	ch := newChain()
	ch.build(os.Stdin)

	for k, v := range ch.stateTab {
		fmt.Println(k, v)
	}

	ch.generate(100)
}
