#include <stdio.h>
#include <stdlib.h>

#include "udp_sock.h"

#define SERVER_PORT 54321
#define CLIENT_PORT 54322

#define N_REPS 100 // Quantidade de repetições de cada teste

int main(int argc, char* argv[])
{
	char* buf;
	int size;
	float rtt;

	if(argc != 2) {
		fprintf(stderr, "Uso: %s <host>\n", argv[0]);
		exit(1);
	}

	if((buf = calloc(BUF_MAX, sizeof(char))) == NULL) {
		fprintf(stderr, "Memória insuficiente\n");
		exit(1);
	}

	// Inicializa socket
	sockinit(argv[1], SERVER_PORT, CLIENT_PORT);

	// Item (a)
	printf("Latência ida-e-volta UDP:\n");

	for(size = 1; size <= 1000; size += (size == 1 ? 99 : 100)) {
		// Envia mensagem e mede RTT
		rtt = sockrtt(buf, size, N_REPS);
		printf("  %4d bytes: %.1f ms\n", size, rtt);
	}

	// Item (b)
	printf("\nVazão UDP:\n");

	for(size = 1; size <= 32; ++size) {
		// Envia mensagem e mede RTT
		rtt = sockrtt(buf, 1024 * size, N_REPS);
		printf("  %2d kB: %.1f kbps\n", size, 1024 * size / rtt);
	}

	sockclose();
	return 0;
}
