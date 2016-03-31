package main

func folowEpsilon(state *State, visited map[string]bool) States {
	if visited[state.Name] {
		return States{}
	}
	visited[state.Name] = true
	// add state
	new := States{state}
	states, ok := state.Transitions[EPSILON]
	if !ok {
		// no more epsilon transitions
		return new
	}
	for _, s := range states {
		// folow epsilon states
		new = append(new, folowEpsilon(s, visited)...)
	}
	return new
}

func FolowEpsilon(state *State) States {
	return folowEpsilon(state, make(map[string]bool))
}

func Closure(old States) States {
	if len(old) == 0 {
		return States{}
	}

	// construct lookup table
	tab := make(map[string]map[string]States)
	for _, s := range old {
		tab[s.Name] = s.Transitions
	}

	state := old.Initial()
	// handle initial state typ, in case
	// we can reach final state thougth epsilons
	for _, s := range FolowEpsilon(state) {
		if s.Typ.Equals(Final) {
			state.Typ |= Final
		}
	}
	new := States{
		// add new initial state
		NewState(state.Name, state.Typ),
	}

	for i := 0; i < len(new); i++ {
		state = new[i]

		transitions := tab[state.Name]
		for symbol, states := range transitions {
			if symbol == EPSILON {
				continue
			}

			e := States{}
			for _, s := range states {
				e = append(e, FolowEpsilon(s)...)
			}
			newState := JoinStates(e)

			// save newState to table
			tab[newState.Name] = newState.Transitions

			// add newState to DFA
			new.Add(NewState(newState.Name, newState.Typ))
			new.Connect(state.Name, symbol, newState.Name)
		}
	}
	return new
}

func NFAToDFA(nfa *Automata) *Automata {
	dfa := &Automata{
		Typ:      DFA,
		Alphabet: nfa.Alphabet,
		States:   Closure(nfa.States),
	}
	dfa.init(dfa.States)
	return dfa
}

func Minimize(states States) States {

	// groups := map[int]States{}
	// ids := map[*State]int{}

	// // divide states to final and others
	// for _, s := range states {
	// 	if s.Typ.Equals(Final) {
	// 		groups[1] = append(groups[1], s)
	// 		ids[s] = 1
	// 	} else {
	// 		groups[0] = append(groups[0], s)
	// 		ids[s] = 0
	// 	}
	// }

	// for {
	// 	newGroups := map[int]States{}
	// 	newIds := map[*State]int{}

	// 	for _, states := range groups {
	// 		if len(states) <= 1 {
	// 			continue
	// 		}
	// 		for i := 1; i < len(states); i++ {
	// 			ok := true
	// 			s0 := states[i-1]
	// 			s1 := states[i]

	// 			// s0(sym) -> group(sx) == s1(sym) -> group(sx)
	// 			for symbol, state := range s0.Transitions {
	// 				sx := state[0]
	// 				id := ids[sx]

	// 				id2 := -1
	// 				if state2, ok := s1.Transitions[symbol]; ok {
	// 					sx2 := state2[0]
	// 					id = ids[sx2]
	// 				}
	// 				if id != id2 {
	// 					ok = false
	// 					break
	// 				}
	// 			}

	// 			// both points in same group
	// 			if ok {

	// 			}
	// 		}
	// 	}
	// 	if len(new) == len(groups) {
	// 		break
	// 	}
	// }
	return states
}

func Minimization(dfa *Automata) *Automata {
	min := &Automata{
		Typ:      DFA,
		Alphabet: dfa.Alphabet,
		States:   Minimize(dfa.States),
	}
	min.init(min.States)
	return min
}
