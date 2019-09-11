package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	m:= make(map[string]int)
	sp:=strings.Split(s," ")
	for i:=0;i<len(sp);i++{
		m[sp[i]]=m[sp[i]]+1
	}
	return m
}

func main() {
	wc.Test(WordCount)
}
