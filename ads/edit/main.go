package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"
	"unicode"
)

const _debug = true

func debug(msg ...interface{}) {
	if _debug {
		log.Println(msg...)
	}
}

func min(a, b, c int) int {
	if a < b {
		if a < c {
			return a
		}
	} else {
		if b < c {
			return b
		}
	}
	return c
}

func Distance(s1, s2 string) int {

	c := make([]int, len(s1)+1)

	for i := 1; i <= len(s1); i++ {
		c[i] = i
	}

	for i := 1; i <= len(s2); i++ {
		c[0] = i
		diag := i - 1
		for j := 1; j <= len(s1); j++ {
			var (
				cost int
				cj   = c[j]
			)
			if s1[j-1] != s2[i-1] {
				cost = 1
			}
			c[j] = min(c[j]+1, c[j-1]+1, diag+cost)
			diag = cj
		}
	}

	return c[len(s1)]
}

type Stats struct {
	total      int
	dictionary int
	corrected  int
}

func (s *Stats) String() string {
	return fmt.Sprintf("\nTotal words checked: %d\n", s.total) +
		fmt.Sprintf("In dictionary: %d\n", s.dictionary) +
		fmt.Sprintf("Corrected: %d", s.corrected)
}

type Corrector func(string) string

func NewCorrector(dictionary map[string]bool) (Corrector, *Stats) {
	corrected := map[string]string{}
	stats := &Stats{}
	fun := func(s string) string {

		stats.total++

		if len(s) == 1 || strings.ContainsAny(s, "0123456789") {
			return s
		}

		runes := make([]rune, 0, len(s))
		for _, r := range s {
			if unicode.IsLetter(r) {
				runes = append(runes, r)
			}
		}

		lower := strings.ToLower(string(runes))

		if dictionary[lower] {
			stats.dictionary++
			return lower
		}

		if v, ok := corrected[lower]; ok {
			return v
		}

		var (
			threshold  = 3
			suggestion = lower
			min        = threshold
		)

		if len(runes) == 0 {
			runes = []rune(s)
			threshold = int(1.5 * float64(len(runes)))
		}

		if unicode.IsUpper(runes[0]) {
			threshold = 1
		}

		if len(runes) < len(s) {
			threshold = int(1.5 * float64(len(s)))
		}

		for word := range dictionary {
			distance := Distance(lower, word)
			if distance <= threshold && distance <= min {
				suggestion = word
				min = distance
			}
		}

		if suggestion != lower {
			stats.corrected++
			corrected[lower] = suggestion
		}

		debug("correcting", s, "->", suggestion)

		return suggestion
	}

	return fun, stats
}

func main() {

	f, err := os.Open("slovnik.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	dictionary := map[string]bool{}

	for {
		var word string
		_, err := fmt.Fscanln(f, &word)

		if err == io.EOF {
			break
		} else if err != nil {
			log.Fatal(err)
		}

		dictionary[word] = true
	}

	var (
		corrector, stats = NewCorrector(dictionary)
		space            = ""
	)

	for {
		var word string
		_, err := fmt.Scan(&word)

		if err == io.EOF {
			break
		} else if err != nil {
			log.Fatal(err)
		}

		fmt.Print(space, corrector(word))
		space = " "
	}

	debug(stats)
}
