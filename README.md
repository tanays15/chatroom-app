# Chatroom App

Basic chatroom app in C using websockets

## Description

This project models a basic chatting app built in C using websockets to communicate between many clients and a server. The clients all connect to a polling server which can assign clients to rooms, remove clients from rooms, and get messages from clients and distribute them to every other client in that room. It supports 3 basic commands "j #", "l", "m 'msg'", to join, leave, and send a message.

## Getting Started

### Dependencies

* clang/gcc to compile project and use C libraries

### Installing

* Clone this repository

### Executing program

* Run `make` in the root directory of the project to compile all files
* In one terminal run the server by using `./server PORT` to start the server, if no port is given, it will default to 8080
* In one or more other terminals use the command `./client PORT` to start the client on the specified port. Unlike the server this will not default to 8080, a port number must be provided.
#### Commands
* "j" will allow clients to join a room, namely "j d", where d is an integer
* "l" will allow clients to leave rooms
* "m 'msg'" will allow clients to send a message to all other clients in the same room as them, e.g. "msg hello"
* All of quotes should be ommited from these commands

## Authors

Tanay Sahasrabudhe
Github: tanays15
