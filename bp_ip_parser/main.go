package main

import (
	"bufio"
	"bytes"
	"fmt"
	"os"
	"regexp"
)

var (
	reInet    = regexp.MustCompile(`(?i:inetnum:.*)`)
	reIP      = regexp.MustCompile(`[0-9]{1,3}`)
	reComment = regexp.MustCompile(`^(%|#)`)
	reSk      = regexp.MustCompile(`(?i:country:\s*sk\b|\S*\.sk\s|\bslovak(ia)?\b|\bslovensko\b)`)
	reEmpty   = regexp.MustCompile(`^$`)
)

func makeCopy(src [][]byte) [][]byte {
	dst := make([][]byte, len(src))
	for i, e := range src {
		dst[i] = make([]byte, len(e))
		copy(dst[i], e)
	}
	return dst
}

func parseSK(sc *bufio.Scanner) chan [][]byte {

	ch := make(chan [][]byte, 1024)
	go func() {
		defer close(ch)
		var buf bytes.Buffer
		for sc.Scan() {
			Line := sc.Bytes()
			if reComment.Match(Line) {
				buf.Reset()
				continue
			} else if reEmpty.Match(Line) {
				if reSk.Match(buf.Bytes()) {
					ch <- makeCopy(reIP.FindAll(reInet.Find(buf.Bytes()), 8))
				}
				buf.Reset()
			} else {
				buf.Write(Line)
				buf.WriteString("\n")
			}
		}
	}()
	return ch
}

func parseIP(ch chan [][]byte) chan struct{} {

	ret := make(chan struct{})
	go func() {
		defer close(ret)
		var buf bytes.Buffer
		for inetNums := range ch {
			buf.Reset()
			for i := 0; i < 4; i++ {
				if bytes.Compare(inetNums[i], inetNums[i+4]) == 0 {
					buf.Write(inetNums[i])
				} else {
					buf.Write(inetNums[i])
					buf.WriteString("-")
					buf.Write(inetNums[i+4])
				}
				if i < 3 {
					buf.WriteString(".")
				}
			}
			fmt.Println(buf.String())
			ret <- struct{}{}
		}
	}()
	return ret
}

// DB
// ftp://ftp.ripe.net/ripe/dbase/split/ripe.db.inetnum.gz
func main() {
	in := bufio.NewScanner(os.Stdin)
	for range parseIP(parseSK(in)) {
	}
}
