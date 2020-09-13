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
	int s, new_s;

	// Monta estrutura de dados de endereço
	bzero((char*) &sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	// Prepara abertura passiva
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

	listen(s, MAX_PENDING);

	// Espera conexão, depois recebe e imprime texto
	while(1) {
		if((new_s = accept(s, (struct sockaddr*) &sin, &len)) < 0) {
			fprintf(stderr, "%s: accept: ", argv[0]);
			perror("");
			exit(1);
		}

		while((len = recv(new_s, buf, sizeof(buf), 0))) {
			fputs(buf, stdout);
		}

		close(new_s);
	}
}
