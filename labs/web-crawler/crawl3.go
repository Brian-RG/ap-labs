// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"os"
	"github.com/todostreaming/gopl.io/ch5/links"
	"flag"
	"strconv"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

type path struct{
	links []string
	level int
}

func crawl(url string, lvl int) path {
	fmt.Println(url)

	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}

	res:=path{links:list,level:lvl+1}
	return res
}

//!-sema

//!+
func main() {
	depth:=flag.Int("depth",1,"the maximum depth to explore")
	flag.Parse()

	fmt.Println("The max depth is",*depth)



	worklist := make(chan path)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	
	cur:=1
	
	start:=path{links:os.Args[1:], level:cur}

	n++
	go func() { worklist <- start }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		urls := <-worklist
		if(urls.level<=*depth){
			fmt.Println("level:",urls.level)
			for _, link := range urls.links {
				if link!="-depth="+strconv.Itoa(*depth){
					if !seen[link] {
						seen[link] = true
						n++
						go func(link string) {
							worklist <- crawl(link,urls.level)
						}(link)
					}	
				}
			}
		}
	}
}

//!-
