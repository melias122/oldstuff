package main

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"strings"
)

type AutomataType string

const (
	DFA AutomataType = "DKA"
	NFA AutomataType = "NKA"
)

type Automata struct {
	Alphabet map[string]bool
	States   []*State
	Typ      AutomataType
}

func NewAutomata(rd io.Reader) (*Automata, error) {
	var (
		NStates   int
		TermCount int
		r         = errReader{r: bufio.NewReader(rd)}
		auto      Automata
	)

	r.ReadLine().Format(&NStates)
	r.ReadLine().Format(&TermCount)

	states := make(map[string]*State) // [state_name]*state
	for i := 0; i < NStates; i++ {
		var name, typ string
		r.ReadLine().Split(" ", &name, &typ)
		states[name] = NewState(name, typ)
	}

	auto.Alphabet = make(map[string]bool)
	for i := 0; i < TermCount; i++ {
		s := r.ReadLine().String()
		auto.Alphabet[s] = true
	}

	for {
		var from, to, symbol string
		r.ReadLine().Split(",", &from, &symbol, &to)
		if r.Error() != nil {
			break
		}
		states[from].AddTransition(symbol, states[to])
	}

	switch r.Error() {
	case io.EOF, nil:
	default:
		return nil, errors.New(inputFile + ": nespravny format suboru")
	}

	auto.Typ = DFA
	auto.States = make([]*State, 0, NStates)
	for _, s := range states {
		auto.States = append(auto.States, s)

		if auto.Typ == DFA {
			for s, t := range s.Transitions {
				if len(t) > 1 || s == EPSILON {
					auto.Typ = NFA
					break
				}
			}
		}
	}

	return &auto, nil
}

func (a *Automata) String() string {
	// s := fmt.Sprintf("%s: {%s}\n", a.Typ, a.Alphabet)
	s := "Automata type: " + string(a.Typ) + "\nAlphabet = {"
	sep := ""
	for alpha := range a.Alphabet {
		s += sep + alpha
		sep = ", "
	}
	s += "}\n"
	for _, st := range a.States {
		s += st.String()
	}
	return s
}

func (a *Automata) InitialState() *State {
	for _, s := range a.States {
		if s.Typ == INIT {
			return s
		}
	}
	return nil
}

func (a *Automata) Accept(word string) bool {

	state := a.InitialState()
	if state == nil {
		return false
	}
	var (
		ok     bool
		states []*State
	)
	for _, ch := range word {
		if states, ok = state.Transitions[string(ch)]; !ok {
			return false
		}
		state = states[0]
	}

	if state.Typ != FINAL {
		return false
	}
	return true
}

type errReader struct {
	r    *bufio.Reader
	err  error
	line string
}

func (e *errReader) String() string {
	var s string
	e.Format(&s)
	return s
}

func (e *errReader) ReadLine() *errReader {
	line, err := e.r.ReadString('\n')
	e.setErr(err)

	if len(line) > 1 {
		line = line[:len(line)-1]
	}
	e.line = line

	return e
}

func (e *errReader) Format(arg interface{}) {
	e.scan(e.line, arg)
}

func (e *errReader) Split(sep string, args ...interface{}) {
	fields := strings.Split(e.line, sep)
	min := len(fields)
	if min > len(args) {
		min = len(args)
	}
	for i := 0; i < min; i++ {
		if len(fields[i]) > 0 {
			e.scan(fields[i], args[i])
		}
	}
}

func (e *errReader) scan(s string, arg interface{}) {
	_, err := fmt.Sscan(s, arg)
	// fmt.Println(err)
	if err != nil {
		fmt.Println(err, s)
	}
	e.setErr(err)
}

func (e *errReader) setErr(err error) {
	if err != nil && e.err == nil {
		e.err = err
	}
}

func (e *errReader) Error() error {
	return e.err
}
