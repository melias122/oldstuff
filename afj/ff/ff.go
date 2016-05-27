package main

import (
	"fmt"
	"log"
	"os"
	"strings"
)

const ε = string('0')

func main() {

	g := NewGrammar()
	if err := g.parse("grammar.ctf"); err != nil {
		log.Fatal(err)
	}

	var (
		first  = FIRST(g)
		follow = FOLLOW(g, first)
		rt     = NewRT(g, first, follow)
	)

	writeRules(g, first, follow, rt)

	fmt.Print("Vstup: ")

	var w string
	fmt.Scanln(&w)

	fmt.Printf("---\nCinnost:\n")

	if LL1(g, w, rt) {
		fmt.Println("AKCEPTUJEM")
	} else {
		fmt.Println("NEAKCEPTUJEM")
	}
}

func writeRules(g Grammar, first, follow map[string]string, rt [][]string) {
	w, err := os.Create("rules.ctf")
	if err != nil {
		log.Fatal(err)
	}
	defer w.Close()

	fmt.Fprintln(w, "FIRST:")
	for _, N := range g.N {
		fmt.Fprintf(w, "%s: %s\n", N, strings.Join(strings.Split(first[N], ""), ","))
	}
	for _, T := range g.T {
		fmt.Fprintf(w, "%s: %s\n", T, strings.Join(strings.Split(first[T], ""), ","))
	}
	fmt.Fprintln(w, "")

	fmt.Fprintln(w, "FOLLOW:")
	for _, N := range g.N {
		fmt.Fprintf(w, "%s: %s\n", N, strings.Join(strings.Split(follow[N], ""), ","))
	}
	// for _, T := range g.T {
	// fmt.Fprintf(w, "%s: %s\n", T, strings.Join(strings.Split(follow[T], ""), ","))
	// }
	fmt.Fprintln(w, "")

	fmt.Fprint(w, "RT: ")
	sep := ""
	for _, T := range g.T {
		fmt.Fprint(w, sep+T)
		sep = " "
	}
	fmt.Fprintln(w, "")
	for i, N := range g.N {
		fmt.Fprint(w, N+": ")
		sep = ""
		for j := range g.T {
			rule := rt[i][j]
			if rule == "" {
				rule = "-"
			}
			fmt.Fprint(w, sep+rule)
			sep = " "
		}
		fmt.Fprint(w, "\n")
	}

}

type Grammar struct {
	N []string
	T []string
	P map[string][]string
}

func NewGrammar() Grammar {
	return Grammar{
		P: make(map[string][]string),
	}
}

func (g *Grammar) parse(path string) error {
	file, err := os.Open(path)

	if err != nil {
		return err
	}
	defer file.Close()

	var (
		nonterminals int
		terminals    int
		rules        int
	)
	if _, err := fmt.Fscanln(file, &nonterminals); err != nil {
		log.Fatal(err)
	}
	if _, err := fmt.Fscanln(file, &terminals); err != nil {
		log.Fatal(err)
	}
	if _, err := fmt.Fscanln(file, &rules); err != nil {
		log.Fatal(err)
	}

	for i := 0; i < nonterminals; i++ {
		var r string
		if _, err := fmt.Fscanln(file, &r); err != nil {
			log.Fatal(err)
		}
		g.N = append(g.N, r)
	}

	for i := 0; i < terminals; i++ {
		var r string
		if _, err := fmt.Fscanln(file, &r); err != nil {
			log.Fatal(err)
		}
		g.T = append(g.T, r)
	}

	for i := 0; i < rules; i++ {
		var s string
		if _, err := fmt.Fscanln(file, &s); err != nil {
			log.Fatal(err)
		}
		sp := strings.Split(s, "->")
		if len(sp) != 2 {
			log.Fatal("parse error: expecting ->", s)
		}
		g.P[sp[0]] = append(g.P[sp[0]], sp[1])
	}

	return nil
}

func (g Grammar) isTerminal(s string) bool {
	for _, t := range g.T {
		if s == t {
			return true
		}
	}
	return false
}

func (g Grammar) isNonTerminal(s string) bool {
	for _, n := range g.N {
		if s == n {
			return true
		}
	}
	return false
}

// FIRST
// Vstup: Redukovaná bezkontextová gramatika G = (N, T, P, S).
// Výstup: Množiny FIRST(X) pre X ∈ N ∪ T
func FIRST(g Grammar) map[string]string {

	fx := make(map[string]string)

	// pre všetky A ∈ N rob
	for _, A := range g.N {
		// 	FIRST(A) ← ∅
		fx[A] = ""
	}

	// pre všetky a ∈ T rob
	for _, a := range g.T {
		// 	FIRST(a) ← {a}
		fx[a] = string(a)
	}

	// pre všetky pravidlá A → aα rob
	for A, P := range g.P {
		// pre kazde pravidlo Neterminalu
		for _, p := range P {
			// 	FIRST(A) ← FIRST(A) ∪ {a}
			if len(p) > 0 && g.isTerminal(string(rune(p[0]))) {
				fx[A] += string(rune(p[0]))
			}
		}
	}

	// pre všetky A ∈ Nε rob
	for A, P := range g.P {
		for _, r := range P {
			if r == ε {
				// 	FIRST(A) ← FIRST(A) ∪ {ε}
				fx[A] += ε
			}
		}
	}

	// opakuj pokial’ zmenila sa niektorá z množín FIRST(A), A ∈ N
	for {

		changed := false

		// 	pre všetky pravidlá A → Bα rob
		for A, P := range g.P {
			// pre kazde pravidlo Neterminalu
			for _, rule := range P {
				if len(rule) > 0 && g.isNonTerminal(string(rune(rule[0]))) {

					fxA := fx[A]

					// FIRST(A) ← FIRST(A) ∪ FIRST(Bα)
					fx[A] = uniqAddStrings(fx[A], FIRSTOnString(fx, rule))

					if fx[A] != fxA {
						changed = true
					}
				}
			}
		}

		if !changed {
			return fx
		}
	}
}

// FIRSTOnString
// Vstup: množiny FIRST(X), X ∈ N ∪ T; ret’azec α ∈ (N ∪ T)∗.
// Výstup: FIRST(α), α ∈ (N ∪ T)∗
func FIRSTOnString(fx map[string]string, s string) string {

	// ak α = ε potom
	if s == ε {
		// 	vrát’ {ε}
		return ε
	}

	// nech α = X0X1...X n-1
	a := []rune(s)

	// 	FIRST(α) ← FIRST(X0) − {ε}
	fa := strings.Replace(fx[string(a[0])], ε, "", -1)

	// 	i ← 0
	i := 0

	// 	pokial’ ε ∈ FIRST(Xi) ∧ i < n rob
	for strings.Contains(fx[string(a[i])], ε) && i < len(a) {
		i++
		// 		FIRST(α) ← FIRST(α) ∪ (FIRST(Xi) − {ε})
		fa += strings.Replace(fx[string(a[i])], ε, "", -1)
	}

	// 	ak i = n ∧ ε ∈ FIRST(Xn) potom
	if i == len(a) {
		// 		FIRST(α) ← FIRST(α) ∪ {ε}
		fa += ε
	}

	return fa
}

// Vstup: Redukovaná bezkontextová gramatika G = (N, T, P, S),
// množiny FIRST(X) pre X ∈ N ∪ T
// Výstup: množiny FOLLOW(A) pre A ∈ N
func FOLLOW(g Grammar, first map[string]string) map[string]string {

	fx := make(map[string]string)

	// pre všetky A ∈ N rob
	for _, A := range g.N {
		// FOLLOW(A) ← ∅
		fx[A] = ""
	}

	// FOLLOW(S) ← {ε}
	//  Počiatočný neterminál má vždy označenie S.
	fx["S"] = ε

	for {
		// fmt.Println(fx)

		changed := false
		// pre všetky B ∈ N v pravých stranách pravidiel A → αBβ rob
		for _, B := range g.N {
			fxB := fx[B]

			for A, rules := range g.P {

				// rule == A → αBβ
				for _, rule := range rules {
					// fmt.Println(A, "->", rule)
					if strings.Contains(rule, B) {

						// prava strana pravidla
						b := strings.Split(rule, B)[1]

						if b == "" {
							b = ε
						}

						// fb <- FIRST(β)
						fb := FIRSTOnString(first, b)

						// FOLLOW(B) ← FOLLOW(B) ∪ (fb - {ε})
						fx[B] = uniqAddStrings(fx[B], strings.Replace(fb, ε, "", -1))

						// ak ε ∈ FIRST(β) potom
						if strings.Contains(fb, ε) {
							// FOLLOW(B) ← FOLLOW(B) ∪ FOLLOW(A)
							fx[B] = uniqAddStrings(fx[B], fx[A])
						}
					}
				}
			}

			if fxB != fx[B] {
				changed = true
			}
		}

		// pokial’ sa zmenila niektorá z množín FOLLOW(B), B ∈ N
		if !changed {
			return fx
		}
	}

	return fx
}

func NewPREDICT(first, folow map[string]string) func(N, P string) string {
	return func(N, P string) string {
		fa := FIRSTOnString(first, P)
		if strings.Contains(fa, ε) {
			return uniqAddStrings(strings.Replace(fa, ε, "", -1), folow[N])
		} else {
			return fa
		}
	}
}

func NewRT(g Grammar, first, follow map[string]string) [][]string {

	rt := make([][]string, len(g.N))
	for i := range rt {
		rt[i] = make([]string, len(g.T))
	}

	indexT := make(map[string]int)
	for i, T := range g.T {
		indexT[T] = i
	}

	predict := NewPREDICT(first, follow)

	n := 1
	for i, N := range g.N {

		for _, P := range g.P[N] {

			for _, T := range predict(N, P) {
				rt[i][indexT[string(T)]] = fmt.Sprint(n)
			}

			n++
		}
	}

	return rt
}

// Vstup: Bezkontextová gramatika G = (N, T, P, S), ret’azec w,
// rozkladová tabul’ka syntaktického analyzátora LL(1) RT
// Výstup: Zistenie, ci ˇ w ∈ L(G) a postupnost’ pravidiel l’avého odvodenia w
func LL1(g Grammar, w string, RT [][]string) bool {

	var (
		n            = 1
		Rule         = make(map[string]string)
		TermIndex    = make(map[string]int)
		NonTermIndex = make(map[string]int)
	)
	for i, N := range g.N {

		NonTermIndex[N] = i

		for _, P := range g.P[N] {
			Rule[fmt.Sprint(n)] = P
			n++
		}
	}

	for i, T := range g.T {
		TermIndex[T] = i
	}

	// epsilon na vstupe
	if w == "" {
		w = ε
	}

	// vlož do zásobníka S
	var (
		stack = []rune("S")
		input = []rune(w)
	)

	// t ← prvá lexéma zo vstupu
	t := string(input[0])
	input = input[1:]

	for len(stack) > 0 {
		// X ← symbol z vrchu zásobníka a zároven ho odstrá ˇ n zo ˇ zásobníka
		X := string(stack[0])
		stack = stack[1:]

		// ak X ∈ N a zárovenˇ RT[X, t] = X → α potom
		if g.isNonTerminal(X) {

			// vlož do zásobníka α aby l’avý krajný symbol bol na vrchu
			E := RT[NonTermIndex[X]][TermIndex[t]]
			if E != "" {
				stack = append([]rune(Rule[E]), stack...)
			} else {
				goto syntaxError
			}

			// pošli na výstup pravidlo X → α
			fmt.Println("E" + E)
		} else if g.isTerminal(X) {

			if X == t {
				fmt.Println("P")
				// t ← d’alšia lexéma zo vstupu
				if len(input) > 0 {
					t = string(input[0])
					input = input[1:]
				}
			} else {
				goto syntaxError
			}

		} else {
			goto syntaxError
		}

	}
	return true

syntaxError:
	// spracuj syntaktickú chybu
	fmt.Println("ZIADNE DOSTUPNE PRAVIDLO")
	return false
}

// adds s1 to s0
func uniqAddStrings(s0 string, s1 string) string {
	new := s0
	for _, r1 := range s1 {
		ok := true
		for _, r2 := range s0 {
			if r1 == r2 {
				ok = false
			}
		}
		if ok {
			new += string(r1)
		}
	}
	return new
}
