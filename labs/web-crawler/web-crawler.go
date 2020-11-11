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
	"flag"

	"gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

// struct for a site
type site struct {
	url string
	depth int
}

// returns a site
func crawl(link site, logFile string, depth int) []site {
	// file
	output, err := os.OpenFile(logFile, os.O_APPEND | os.O_CREATE | os.O_WRONLY, 0644)
	// file validation
	if err != nil {
		fmt.Printf("Error on file: %s\n", err)
	}

	if _, err := output.WriteString(link.url + "\n"); err != nil {
		fmt.Printf("Error: %s\n", err)
	}

	output.Close()

	if link.depth < depth {
		tokens <- struct{}{} // acquire a token
		list, err := links.Extract(link.url)
		tempLink := make([]site, len(list))
		for i, url :=  range list {
			tempLink[i] = site{url: url, depth: link.depth + 1}
		}
		<-tokens // release the token

		if err != nil {
			log.Print(err)
		}
		return tempLink
	}

	return []site{}
}


//!-sema

//!+
func main() {
	if len(os.Args) < 4 {
		fmt.Println("Error receving arguments")
		fmt.Println("Try: ./web-crawler -depth=2 -results=test1.txt https://google.com/")
		os.Exit(1)
	}
	worklist := make(chan []site)
	var n int // number of pending sends to worklist
	var depth = flag.Int("depth", 1, "depth")
	var logFile = flag.String("results", "results.txt", "log file")
	flag.Parse()

	firstLink := os.Args[3:]
	//graph
	links := make([]site, len(firstLink))
	for i, url := range firstLink {
		links[i] = site{url: url, depth: 0}
	}
	// Start with the command-line arguments.
	n++
	go func() { worklist <- links }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link.url] {
				seen[link.url] = true
				n++
				go func(link site) {
					worklist <- crawl(link, *logFile, *depth)
				}(link)
			}
		}
	}
}

//!-
