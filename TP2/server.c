#include <stdio.h>
#include <stdlib.h>

#include "udp_sock.h"

#define SERVER_PORT 54321
#define CLIENT_PORT 54322

int main(int argc, char* argv[])
{
	char buf[BUF_MAX];
	int size;

	if(argc != 2) {
		fprintf(stderr, "Uso: %s <host>\n", argv[0]);
		exit(1);
	}

	// Inicializa socket
	sockinit(argv[1], CLIENT_PORT, SERVER_PORT);

	// Continuamente retransmite mensagens e imprime tamanho
	while((size = sockrecv(buf)) > 0) {
		socksend(buf, size);
		printf("Recebidos %d bytes\n", size);
	}

	sockclose();
	return 0;
}
