// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"fmt"
	"os"
)

func handleConn(c net.Conn) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	//validation
	if len(os.Args) != 3{
		fmt.Println("ERROR: Invalid arguments input!")
		os.Exit(1)
	}

	args := os.Args

	if args[1] != "-port"{
		fmt.Println("Command -port missing")
		fmt.Println("Example: TZ=US/Eastern    go run clock2.go -port 8010")
	}

	port := "localhost:"+args[2]
	listener, err := net.Listen("tcp", port)
	fmt.Println("Clock server initialized on port: ", port);
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}
