#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SERVER_PORT 54321
#define MAX_LINE 256

int main(int argc, char* argv[])
{
	struct sockaddr_in sin;
	char* host;
	char buf[MAX_LINE];
	int s;
	int len;

	if(argc == 2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "Uso: %s <host>\n", argv[0]);
		exit(1);
	}

	// Monta estrutura de dados do endereço
	bzero((char*) &sin, sizeof(sin));

	if(inet_aton(host, &sin.sin_addr) == 0) {
		fprintf(stderr, "Host %s desconhecido ou inválido\n", argv[0]);
		exit(1);
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);

	// Abertura ativa
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "%s: socket: ", argv[0]);
		perror("");
		exit(1);
	}

	if(connect(s, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		fprintf(stderr, "%s: connect: ", argv[0]);
		perror("");
		close(s);
		exit(1);
	}

	// Laço principal: obtém e envia linhas de texto
	while(fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = 0;
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
	}

	close(s);
}
