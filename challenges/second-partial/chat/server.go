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
	"flag"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct{
	canal client
	name string
	add string
	adm bool
	connection net.Conn
}

var usuarios=[]user{}

var (
	entering = make(chan user)
	leaving  = make(chan user)
	messages = make(chan string) // all incoming client messages
	administrador=false
	ban = make(chan user)
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			
			clients[cli.canal]=true

		case cli := <-leaving:
			delete(clients, cli.canal)
			close(cli.canal)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)
	ident:="irc-server > "
	input := bufio.NewScanner(conn)
	new:=0
	who:=conn.RemoteAddr().String()
	u:=user{}
	for input.Scan() {
		if(new==0){
			who=input.Text()
			ch <- ident+"Your user :" + who + " is successfully logged in!"
			messages <- who + " has arrived"		

			u.canal,u.name,u.add,u.adm,u.connection=ch,who,conn.RemoteAddr().String(),false,conn

			fmt.Println(ident+"New connected user ",u.name)
			if(!administrador){
				fmt.Println(ident+u.name+" was promoted as ADMIN")
				u.adm=true
				u.canal <- ident+"Congrats, you are the first user"
				u.canal <- ident+"You are the server admin"
				administrador=true
				
			}
			usuarios=append(usuarios,u)
			entering <- u
			new=-1
		}else{
			mes:=input.Text()
			command:=strings.Split(mes," ")
			switch command[0]{

			case "/user":
				if(len(command)==2){
					us:=command[1]
					response:="Error, no such user"
					for usuario:= range usuarios{
						if(usuarios[usuario].name==us){
							response=getUserinfo(usuarios[usuario])
						}
					}
					u.canal<-ident+response
				}else{
					u.canal<-ident+"Error, missing user you want to get info from"
				}
				
				break
			
			case "/time":
				u.canal<- ident+getTime()
				break

			case "/users":
				list:=""
				for x:=range usuarios{
					list+=(usuarios[x].name+", ")
				}
				u.canal<-ident+list[0:len(list)-2]
				break
			
			case "/kick":
				if(u.adm){
					if(len(command)>=2){
						us:=command[1]
						flag:=false
						for usuario:= range usuarios{
							if(usuarios[usuario].name==us){
								kicked:=usuarios[usuario]
								kicked.canal<-ident+"You are kicked from this channel"
								kicked.canal<-ident+"Bad language is not allowed"
								fmt.Println(ident+kicked.name+" was kicked")
								usuarios[usuario].connection.Close()
								ban<-usuarios[usuario]
								messages<-ident+us+" was kicked from channel for bad language policy violation"
								flag=true
								break
							}
						}
						if(!flag){
							u.canal<-ident+"Error, no such user"
						}
					}else{u.canal<-ident+"Error, missing user you want to kick"}
				}else{u.canal<-"Error, you are not the admin."}
				break

			case "/msg":
				if(len(command)>=2){
					us:=command[1]
					mensaje:=strings.Join(command[2:len(command)]," ")
					flag=false
					for usuario:= range usuarios{
						if(usuarios[usuario].name==us){
							usuarios[usuario].canal<-ident+u.name+" direct:"+ mensaje
							flag=true
							break
						}
					}
					if(!flag){
						u.canal<-ident+"Error, no such user"
					}
				}else{u.canal<-ident+"Error, missing user you want to send the message"}
				break

			default:
				messages <- who + ": " + mes
				break
			}
		}
	}


	select{
		case <-ban:
			leaving <- u
			conn.Close()
		default:
			leaving <- u
			messages <- who + " has left"
			fmt.Println(ident+who+" left")
			conn.Close()
	}
	
}
func getUserinfo(usuario user) string{
	return "username: "+usuario.name+" IP: "+usuario.add

}
func getTime() string {
	return "Local Time: America/Mexico_City "+time.Now().Format("15:04")
}
func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) 
	}
}
//!-handleConn

//!+main
func main() {
	
	direction:=flag.String("host","localhost","an address")
	port:=flag.String("port","8000","a port number")
	flag.Parse()
	add:=strings.Join([]string{*direction,*port}, ":")
	listener, err := net.Listen("tcp", add)
	if err != nil {
		log.Fatal(err)
	}
	name:="irc-server >"
	fmt.Println(name,"Simple IRC Server started at",add)
	fmt.Println(name,"Ready for receiving new clients")
	go broadcaster()
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
