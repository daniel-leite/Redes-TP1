all : client server

client : client.c udp_sock.c
	gcc -Wall -Wextra client.c udp_sock.c -o client

server : server.c udp_sock.c
	gcc -Wall -Wextra server.c udp_sock.c -o server
