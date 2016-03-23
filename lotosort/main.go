package main

import (
	"encoding/csv"
	"flag"
	"log"
	"os"
	"sort"
	"strconv"
)

const (
	comma = ';'
)

var (
	n    int
	path string
)

func init() {
	flag.IntVar(&n, "n", 0, "Sets n columns")
	flag.StringVar(&path, "f", "", "Path to file")
	// flag.StringVar(&path, "-path", "", "Path to file")
}

func Sort(n int, records [][]string, skipHeader bool) [][]string {
	var (
		i, from int
		numbers = make([]int, n)
	)
	if skipHeader {
		records[0] = records[0][:3+n]
		from = 1
		i = 1
	}
	for _, r := range records[from:] {
		for j, str := range r[3 : 3+n] {
			number, err := strconv.Atoi(str)
			if err != nil {
				log.Fatal(err)
			}
			numbers[j] = number
		}

		sort.Ints(numbers)

		// prevedenie intov na stringy
		for j, number := range numbers {
			records[i][j+3] = strconv.Itoa(number)
		}

		records[i] = records[i][:3+n]
		i++
	}

	return records
}

func main() {
	flag.Parse()

	if n <= 0 || path == "" {
		flag.Usage()
		os.Exit(1)
	}

	var (
		records [][]string
		file    *os.File
		err     error
	)

	if file, err = os.Open(path); err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	reader := csv.NewReader(file)
	writer := csv.NewWriter(os.Stdout)

	// nastavenie delimitera
	reader.Comma = comma
	writer.Comma = comma

	if records, err = reader.ReadAll(); err != nil {
		log.Fatal(err)
	}

	records = Sort(n, records, true)

	if err = writer.WriteAll(records); err != nil {
		log.Fatal(err)
	}
}
