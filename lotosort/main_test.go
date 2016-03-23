package main

import "testing"

func TestSort(t *testing.T) {
	in := [][]string{
		[]string{"1", "123", "1", "1", "2", "5", "3", "7", "asdf"},
	}
	want := [][]string{
		[]string{"1", "123", "1", "1", "2", "3", "5", "7"},
	}

	out := Sort(5, in)

	for i := range out {
		for j := range out[i] {
			if out[i][j] != want[i][j] {
				t.Errorf("Got: %s (pos %d), want: %s", out[i], j+1, want[i])
			}
		}
	}
}
