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
	Initial *State

	Alphabet map[string]bool
	States   States
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

	states := States{}
	for i := 0; i < NStates; i++ {
		var name, typ string
		r.ReadLine().Split(" ", &name, &typ)
		state := NewState(name, NewStateType(typ))
		states.Add(state)
	}

	auto.Alphabet = make(map[string]bool)
	for i := 0; i < TermCount; i++ {
		s := r.ReadLine().String()
		auto.Alphabet[s] = true
	}

	for r.Error() == nil {
		var from, to, symbol string
		r.ReadLine().Split(",", &from, &symbol, &to)
		states.Connect(from, symbol, to)
	}

	switch r.Error() {
	case io.EOF:
	default:
		return nil, errors.New(inputFile + ": nespravny format suboru")
	}

	auto.init(states)
	return &auto, nil
}

func (a *Automata) init(states States) {
	a.States = states
	for _, s := range states {
		if s.Typ.Equals(Initial) {
			a.Initial = s
			break
		}
	}
	a.Typ = DFA
	for _, s := range states {
		if a.Typ == NFA {
			break
		}
		for s, t := range s.Transitions {
			if len(t) > 1 || s == EPSILON {
				a.Typ = NFA
				break
			}
		}
	}
}

func (a *Automata) String() string {
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

func (a *Automata) WriteTo(w io.Writer) error {
	fmt.Fprintln(w, len(a.States))
	fmt.Fprintln(w, len(a.Alphabet))
	for _, s := range a.States {
		fmt.Fprintln(w, s.Name, s.Typ)
	}
	for ch := range a.Alphabet {
		fmt.Fprintln(w, ch)
	}
	for _, s := range a.States {
		s.WriteTo(w)
	}
	return nil
}

func (a *Automata) Accept(s string) bool {

	state := a.Initial
	if state == nil {
		return false
	}
	var (
		ok     bool
		states []*State
	)
	for _, ch := range s {
		if states, ok = state.Transitions[string(ch)]; !ok {
			return false
		}
		state = states[0]
	}
	return state.Typ.Equals(Final)
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
	_, err := fmt.Sscanln(s, arg)
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
