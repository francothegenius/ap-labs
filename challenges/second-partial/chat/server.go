// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct {
	channel client
	name string
	ip string
	conn net.Conn
	connectionTime string
	isAdmin bool
}

var (
	users  = make(map[client]*user) // all connected clients
	entering = make(chan user)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
)

func broadcaster() {
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range users {
				cli <- msg
			}

		case clientInfo := <-entering:
			if len(users) == 0 {
				clientInfo.isAdmin = true
			}
			users[clientInfo.channel] = &clientInfo

		case cli := <-leaving:
			delete(users, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	var validUsername = true
	// takes the input
	input := bufio.NewScanner(conn)
	input.Scan()
	inputUsername := input.Text()
	for _,p := range users {
		if p.name == inputUsername {
			validUsername = false
			fmt.Fprintln(conn, "OOPS! Username already taken, try with another username") // NOTE: ignoring network errors
			conn.Close()
			break;
		}
	}

	// if the username is not taken
	if validUsername {
		ch := make(chan string) // outgoing client messages
		go clientWriter(conn, ch)
		// remote address of client
		ip := conn.RemoteAddr().String()
		timeConnected := time.Now().Format("2006-01-02 15:04:05")
		ch <- "irc-server > Welcome to the Simple IRC Server"
		ch <- "irc-server > Your user [" + inputUsername + "] is successfully logged"
		fmt.Println("irc-server > New connected user [" + inputUsername + "]")
		messages <- "irc-server > New connected user [" + inputUsername + "]"
		entering <- user{ch, inputUsername, ip, conn, timeConnected, false}
		// admin check
		if users[ch].isAdmin == true{
			ch <- "irc-server > Congrats, you were the first user."
			fmt.Println("irc-server > ["+ inputUsername + "] was promoted as the channel ADMIN")
			users[ch].isAdmin = true
			ch <- "irc-server > You're the new IRC Server ADMIN"
		}

		// input 
		for input.Scan() { // Incoming client messages
			//receive input
			inputMsg := input.Text()

			//if input is not empty
			if inputMsg != "" {
				if inputMsg[0] == '/' { 
					//split input by space to check which command
					command := strings.Split(inputMsg, " ")

					switch command[0] {
					//users	
					case "/users":
						for _,currentClient := range users {
							ch <- "irc-server > " + currentClient.name + " - connected since " + currentClient.connectionTime
						}
						

					case "/msg":
						if len(command) < 3 {
							ch <- "irc-server > Invalid input, Try: /msg <user> <msg>"
						} else {
							var userFound = false
							for _,currentClient := range users {
								if currentClient.name == command[1] {
									userFound = true
									//inputMsg[6+len(p.name):] getting the message, skipping command and name of user
									currentClient.channel <- inputUsername + " (Private Message) > " + inputMsg[6+len(currentClient.name):]
									break;
								}
							}
							if !userFound {
								ch <- "irc-server > OOPS! user [" + command[1]+ "] not Found"
							}
						}

					case "/time":
						location,_ := time.LoadLocation("Local")
						loc := location.String()
						if loc == "Local"{
							curr,_ := time.LoadLocation("America/Mexico_City")
							loc = curr.String()
						}
						ch <- "irc-server > Local Time: " + loc + " " + time.Now().Format("15:04")

					case "/user":
						if len(command) != 2 {
							ch <- "irc-server > Invalid input, Try: /user <user>"
						} else {
							var userFound = false
							for _,currentClient := range users {
								if currentClient.name == command[1] {
									userFound = true
									ch <- "irc-server > username: " + currentClient.name + ", IP: " + currentClient.ip + " Connected since: " + currentClient.connectionTime
									break;
								}
							}
							if !userFound {
								ch <- "irc-server > OOPS! user [" + command[1]+ "] not Found"
							}
						}

					case "/kick":
						//check if admin
						if users[ch].isAdmin {
							if len(command) != 2 {
								ch <- "irc-server > Invalid input, Try: /kick <user>"
							} else {
								var userFound = false
								for _,currentClient := range users {
									if currentClient.name == command[1] {
										userFound = true
										currentClient.channel <- "irc-server > You're kicked from this channel"
										leaving <- currentClient.channel
										fmt.Println("irc-server > [" + currentClient.name + "] was kicked")
										messages <- "irc-server > [" + currentClient.name + "] was kicked from channel"
										currentClient.conn.Close()
										break;
									}
								}
								if !userFound {
									ch <- "irc-server > OOPS! user [" + command[1]+ "] not Found"
								}
							}
						} else {
							ch <- "irc-server > OOPS!, you need to be an admin in order to kick"
						}
					default:
						ch <- "irc-server > Error! command not found"
					}
				} else { // if is not command, is message
					messages <- inputUsername + " > " + inputMsg
				}
			}
		}
		// if client disconnects
		if users[ch] != nil {
			leaving <- ch
			fmt.Println("irc-server > [" + inputUsername + "] left")
			messages <- "irc-server > [" + inputUsername + "] left channel"
			conn.Close()
		}
	}
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}
//!-handleConn

//!+main
func main() {
	if len(os.Args) != 5 {
		log.Fatal("Error in parameters, usage: go run client.go -host [host] -port [port]")
	}
	server := os.Args[2] + ":" + os.Args[4]
	listener, err := net.Listen("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("irc-server > Simple IRC Server started at " + server)
	go broadcaster()
	fmt.Println("irc-server > Ready for receiving new clients")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main