package main

import (
	"flag"
	"fmt"
	"os"
)

const (
	inputFile  = "input_automat.fsa"
	outputFile = "output_automat.fsa"
)

var (
	opt1 = flag.Bool("1", false, "Simuluj slovo pomocou DKA")
	opt2 = flag.Bool("2", false, "Preved NKA na DKA")
	opt3 = flag.Bool("3", false, "Minimalizuj DKA")
)

func fatal(s string) {
	fmt.Fprintln(os.Stderr, s)
	os.Exit(1)
}

func acceptWord(auto *Automata) {
	if auto.Typ != DFA {
		auto = NFAToDFA(auto)
	}

	var word string
	fmt.Fscanln(os.Stdin, &word)

	if auto.Accept(word) {
		fmt.Println("akceptuje")
	} else {
		fmt.Println("neakceptuje")
	}
}

func convert(old *Automata) {
	new := NFAToDFA(old)
	f, err := os.Create(outputFile)
	if err != nil {
		fatal("Nepodarilo sa otvorit subor " + outputFile)
	}
	defer f.Close()
	new.WriteTo(f)
}

func minimalize(auto *Automata) {

}

func main() {
	flag.Parse()

	if flag.NFlag() != 1 {
		flag.Usage()
		os.Exit(1)
	}

	f, err := os.Open(inputFile)
	if err != nil {
		fatal("Nepodarilo sa nacitat subor " + inputFile)
	}
	defer f.Close()

	auto, err := NewAutomata(f)
	if err != nil {
		fatal(err.Error())
	}

	switch {
	case *opt2:
		convert(auto)
	case *opt3:
		minimalize(auto)
	case *opt1:
		acceptWord(auto)
	default:
		return
	}
}
