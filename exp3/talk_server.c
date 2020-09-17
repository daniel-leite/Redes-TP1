#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 54321
#define MAX_PENDING 5
#define MAX_LINE 256

int main(int argc, char* argv[])
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	unsigned int len = sizeof(sin);
	int s;

	// Monta estrutura de dados de endereço
	bzero((char*) &sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	// Prepara abertura passiva
	if((s = socket(AF_INET, SOCK_DGRAM , 0)) < 0) {
		fprintf(stderr, "%s: socket: ", argv[0]);
		perror("");
		exit(1);
	}

	if((bind(s, (struct sockaddr*) &sin, sizeof(sin))) < 0) {
		fprintf(stderr, "%s: bind: ", argv[0]);
		perror("");
		close(s);
		exit(1);
	}

	// Recebe novas mensagens e imprime
	while((len = recv(s, buf, sizeof(buf), 0))) {
		fputs(buf, stdout);
	}
}
