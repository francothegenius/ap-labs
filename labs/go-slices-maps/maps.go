package main

import (
	"golang.org/x/tour/wc"
	"strings"
	//"fmt"
)

func WordCount(s string) map[string]int {
	sentArr := strings.Fields(s)
	//fmt.Println(sentArr);
	m := make(map[string]int)
	//not assinging index
	//range goes with respect sentArr
	//v takes each word
	for _, v := range sentArr {
		m[v]+=1
	}
	return m
}

func main() {
	wc.Test(WordCount)
}
