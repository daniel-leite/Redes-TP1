#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#define BUF_MAX 4096 // 4 kB

static int s;
static struct sockaddr_in mysock, destsock;

// Inicializa socket
void sockinit(char* addr, int destport, int myport)
{
	// Monta estruturas de dados dos endereços
	bzero((char*) &destsock, sizeof(destsock));
	bzero((char*) &mysock, sizeof(mysock));

	if(inet_aton(addr, &destsock.sin_addr) == 0) {
		fprintf(stderr, "Host %s desconhecido ou inválido\n", addr);
		exit(1);
	}

	destsock.sin_family = AF_INET;
	destsock.sin_port = htons(destport);

	mysock.sin_addr.s_addr = INADDR_ANY;
	mysock.sin_family = AF_INET;
	mysock.sin_port = htons(myport);

	// Abertura ativa
	if((s = socket(AF_INET, SOCK_DGRAM , 0)) < 0) {
		fprintf(stderr, "Erro em socket: ");
		perror("");
		exit(1);
	}

	if(bind(s, (struct sockaddr*) &mysock, sizeof(mysock)) < 0) {
		fprintf(stderr, "Erro em bind: ");
		perror("");
		close(s);
		exit(1);
	}
}

// Encerra socket
void sockclose()
{
	close(s);
}

// Envia dados ao socket inicializado
void socksend(char* buf, int len)
{
	sendto(s, buf, len, 0, (struct sockaddr*) &destsock, sizeof(destsock));
}

// Recebe dados do socket inicializado
int sockrecv(char* buf)
{
	return recv(s, buf, BUF_MAX, 0);
}

// Envia dados e aguarda retorno, reps vezes. Retorna RTT em ms
float sockrtt(char* buf, int tam, int reps)
{
	int i;
	time_t t0 = time(NULL);

	for(i = 0; i < reps; ++i) {
		socksend(buf, tam);
		sockrecv(buf);
	}

	return 1000. * (time(NULL) - t0) / reps;
}
