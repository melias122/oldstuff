package main

import (
	"fmt"
	"io"
	"sort"
)

const (
	EPSILON = "ε"
)

type StateType uint

const (
	Initial StateType = 1 << iota
	None
	Final
)

func NewStateType(st string) StateType {
	switch st {
	case "I":
		return Initial
	case "F":
		return Final
	case "IF":
		return Initial | Final
	default:
		return None
	}
}
func (s StateType) Equals(o StateType) bool { return s&o > 0 }

func (t StateType) String() string {
	switch t {
	case Initial:
		return "I"
	case Final:
		return "F"
	case Initial | Final:
		return "IF"
	default:
		return ""
	}
}

type State struct {
	Name        string
	Typ         StateType
	Transitions map[string]States
}

func NewState(name string, typ StateType) *State {
	return &State{
		Name:        name,
		Typ:         typ,
		Transitions: make(map[string]States),
	}
}

func JoinStates(s States) *State {
	states := map[string]bool{}
	joined := States{}
	for _, s := range s {
		if !states[s.Name] {
			joined = append(joined, s)
			states[s.Name] = true
		}
	}
	sort.Sort(byName{joined})
	var (
		name string
		typ  StateType
		sep  string
	)
	for _, s0 := range joined {
		name += sep + s0.Name
		if s0.Typ.Equals(Final) {
			typ = Final
		}
		sep = "-"
	}

	new := NewState(name, typ)
	for _, s0 := range joined {
		for symbol, state := range s0.Transitions {
			new.AddTransition(symbol, state...)
		}
	}
	return new
}

func (s *State) AddTransition(symbol string, state ...*State) {
	if symbol == "" {
		symbol = EPSILON
	}
	t, ok := s.Transitions[symbol]
	if !ok {
		s.Transitions[symbol] = States{}
	}
	for _, s := range state {
		if _, ok := t.Find(s.Name); !ok {
			t = append(t, s)
		}
	}
	s.Transitions[symbol] = t
}

func (s State) String() string {
	var str string
	str += fmt.Sprintf("%s %s\n", s.Name, s.Typ)

	for k, v := range s.Transitions {
		sep := ""
		str += " |-- " + string(k) + " -> {"
		for _, st := range v {
			str += fmt.Sprintf("%s%s", sep, st.Name)
			sep = ", "
		}
		str += "}\n"
	}
	return str
}

func (s State) WriteTo(w io.Writer) {
	for symbol, s0 := range s.Transitions {
		fmt.Fprintf(w, "%s,%s,%s\n", s.Name, symbol, s0[0].Name)
	}
}

type States []*State

func (s *States) Add(state *State) {
	for i := 0; i < s.Len(); i++ {
		if (*s)[i].Name == state.Name {
			return
		}
	}
	*s = append(*s, state)
}

func (s *States) Connect(s0, symbol, s1 string) {
	if a, ok := s.Find(s0); ok {
		if b, ok := s.Find(s1); ok {
			a.AddTransition(symbol, b)
		}
	}
}

func (s States) Find(name string) (*State, bool) {
	for _, state := range s {
		if state.Name == name {
			return state, true
		}
	}
	return nil, false
}

func (s States) Initial() *State {
	for _, s := range s {
		if s.Typ.Equals(Initial) {
			return s
		}
	}
	return nil
}

func (s States) Len() int      { return len(s) }
func (s States) Swap(i, j int) { s[i], s[j] = s[j], s[i] }

type byName struct{ States }

func (b byName) Less(i, j int) bool { return b.States[i].Name < b.States[j].Name }
