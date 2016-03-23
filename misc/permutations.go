package main

import (
	"fmt"
)

func main() {
	s := []int{1, 2, 3}
	Permute(s)
}

func Permute(arr []int) {

	var rc func(int, int)

	rc = func(i int, n int) {
		if i == n {
			fmt.Println(arr)
		} else {
			for j := i; j <= n; j++ {
				arr[i], arr[j] = arr[j], arr[i]
				rc(i+1, n)
				arr[i], arr[j] = arr[j], arr[i]
			}
		}
	}

	rc(0, len(arr)-1)
}
