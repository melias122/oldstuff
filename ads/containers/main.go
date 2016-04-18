package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"time"
)

type word struct {
	str string
	p   int
}

type wordSorter []word

func (w wordSorter) Len() int           { return len(w) }
func (w wordSorter) Less(i, j int) bool { return w[i].str < w[j].str }
func (w wordSorter) Swap(i, j int)      { w[i], w[j] = w[j], w[i] }

func createInput(path string, nWords int) ([]word, []word) {
	f, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	var (
		uniq       = make(map[string]int)
		dictionary []word
	)
	for {
		var (
			n int
			w string
		)
		if _, err := fmt.Fscanf(f, "%d %s\n", &n, &w); err == io.EOF {
			break
		}
		if _, ok := uniq[w]; !ok {
			uniq[w] = n
			dictionary = append(dictionary, word{str: w, p: n})
		}
	}

	words := make([]word, nWords)
	for i := range words {
		words[i] = dictionary[i]
	}
	return words, dictionary
}

func createTestInput(path string) []string {
	vzorka, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}
	defer vzorka.Close()

	var s []string
	for {
		var w string
		if _, err := fmt.Fscanln(vzorka, &w); err == io.EOF {
			break
		}
		s = append(s, w)
	}
	return s
}

type container interface {
	Contains(string) bool
}

func test(c container, input []string) {
	var (
		t0   = time.Now()
		hits int
	)
	for _, w := range input {
		if c.Contains(w) {
			hits++
		}
	}
	fmt.Printf("%.0f %d\n", time.Since(t0).Seconds(), hits)
}

func main() {

	nWords := 1000
	words, dictionary := createInput("slovnik.txt", nWords)

	t0 := time.Now()
	o := NewOBVS(words, dictionary)
	// read dictionary, construct obvs, print time
	fmt.Printf("%.0f\n", time.Since(t0).Seconds())

	// read test input
	input := createTestInput("vzorka.txt")

	// create hash table, trie
	h := NewHashTableSize(nWords)
	t := NewTrie()

	// fill hash, trie
	for _, w := range words {
		h.Put(w.str, w.p)
		t.Put(w.str)
	}

	test(o, input)
	test(t, input)
	test(h, input)
}
