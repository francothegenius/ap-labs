package main

import (
	"os"
	"fmt"
	"log"
	"net"
	"strings"
)

func main() {
	if len(os.Args) < 2{
		fmt.Println("ERROR: Invalid arguments input!")
		os.Exit(1)
	}

	ch := make(chan int)
	for i := 1; i < len(os.Args); i++ {
		input := strings.Split(os.Args[i], "=")
		timeZone := input[0]
		port := input[1]
		c, err := net.Dial("tcp", port)
		if err != nil{
			log.Fatal(err)
		}

		go handlec(c, timeZone, ch)
	}
	<-ch
}

func handlec(c net.Conn, timeZone string, ch chan int){
	for {
		data := make([]byte, 12)
		_, err := c.Read(data)

		if err != nil{
			c.Close()
			log.Fatal(err)
			return
		}

		fmt.Printf("%s\t: %s", timeZone, data)
	}
	ch <- 1
}