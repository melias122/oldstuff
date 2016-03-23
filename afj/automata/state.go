package main

import "fmt"

const (
	EPSILON = "ε"
)

type StateType string

const (
	INIT  StateType = "I"
	FINAL StateType = "F"
)

type State struct {
	Name        string
	Typ         StateType
	Transitions map[string][]*State
}

func NewState(name string, typ string) *State {
	return &State{
		Name:        name,
		Typ:         StateType(typ),
		Transitions: make(map[string][]*State),
	}
}

func (s *State) AddTransition(symbol string, state *State) {
	var (
		ok bool
		t  []*State
	)
	if symbol == "" {
		symbol = EPSILON
	}
	if t, ok = s.Transitions[symbol]; !ok {
		s.Transitions[symbol] = []*State{}
	}
	t = append(t, state)
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
