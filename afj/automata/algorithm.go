package main

func FolowEpsilon(state *State, visited map[*State]bool) States {
	if visited[state] {
		return States{}
	}
	// add state
	new := States{state}
	states, ok := state.Transitions[EPSILON]
	if !ok {
		// no more epsilon transitions
		return new
	}
	visited[state] = true
	for _, s := range states {
		// folow epsilon states
		new = append(new, FolowEpsilon(s, visited)...)
	}
	return new
}

func Closure(old States) States {
	if len(old) == 0 {
		return States{}
	}

	// construct lookup table with epsilon*
	tab := make(map[string]map[string]States)
	for _, s := range old {
		// add new epsilon* transitions
		es := FolowEpsilon(s, make(map[*State]bool))
		new := JoinStates(es)
		s.Transitions[EPSILON] = States{new}
		tab[s.Name] = s.Transitions
	}

	state := old.Initial()
	new := States{
		NewState(state.Name, state.Typ),
	}
	for i := 0; i < len(new); i++ {
		state = new[i]

		transitions := tab[state.Name]
		for symbol, states := range transitions {
			if symbol == EPSILON {
				continue
			}
			for _, s := range states {
				es := s.Transitions[EPSILON]
				new.Add(NewState(es., typ))
				state.AddTransition(symbol, es...)
			}
		}

		// for symbol, s := range tab[state.Name] {
		// 	t := JoinStates(s)
		// 	if _, ok := tab[t.Name]; !ok {
		// 		tab[t.Name] = t.Transitions
		// 		new = append(new, NewState(t.Name, t.Typ))
		// 	}
		// 	state.AddTransition(symbol, t)
		// }
		// state = tab[new[i].Name]
		// 	for symbol := range s0.Transitions {
		// 		if symbol == EPSILON {
		// 			continue
		// 		}
		// 		states := FolowEpsilon(s0, make(map[*State]bool))
		// 		sort.Sort(byName{states})
		// 		name, typ := states.NameType()

		// 		state, ok := new.Find(name)
		// 		if !ok {
		// 			state = NewState(name, typ)
		// 			new = append(new, state)
		// 		}
		// 		new[i].AddTransition(symbol, state)
		// 	}
	}
	return new
}

func NFAToDFA(nfa *Automata) *Automata {
	dfa := &Automata{
		Typ:      DFA,
		Alphabet: nfa.Alphabet,
		States:   Closure(nfa.States),
	}
	return dfa
}

func Minimal() {

}
