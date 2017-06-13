package main

import (
	"fmt"
	"strings"
)

type Filter interface {
	Process(string) string
}

type Args map[string]string

type Substitution struct {
	r *strings.Replacer
}

func (s *Substitution) Process(text string) string {
	if s.r == nil {
		return text
	}
	return s.r.Replace(text)
}

func NewSubstitution(args Args) Filter {
	r := []string{}
	for k, v := range args {
		r = append(r, k, v)
	}
	return &Substitution{
		r: strings.NewReplacer(r...),
	}
}

type Reverse struct {
}

func NewReverse(Args) Filter {
	return &Reverse{}
}

func (r *Reverse) Process(text string) string {
	runes := []rune(text)
	for i, j := 0, len(runes)-1; i < j; i, j = i+1, j-1 {
		runes[i], runes[j] = runes[j], runes[i]
	}
	return string(runes)
}

type Creator func(Args) Filter

type factory struct {
	factories map[string]Creator
}

var Factory = factory{
	factories: map[string]Creator{
		"Substitution": NewSubstitution,
		"Reverse":      NewReverse,
	},
}

func (f *factory) Create(filter string, args Args) Filter {
	if filter, ok := f.factories[filter]; ok {
		return filter(args)
	}
	return nil
}

func main() {
	filters := []Filter{
		Factory.Create("Substitution", Args{"Some": "Any"}),
		Factory.Create("Substitution", Args{"A": "Ma"}),
		Factory.Create("Reverse", Args{}),
	}

	text := "Some text"
	for _, filter := range filters {
		if filter != nil {
			text = filter.Process(text)
		}
	}
	fmt.Println(text)
}
