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
	struct sockaddr_in server, client;
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

	// Monta estruturas de dados dos endereços
	bzero((char*) &server, sizeof(server));
	bzero((char*) &client, sizeof(client));

	if(inet_aton(host, &server.sin_addr) == 0) {
		fprintf(stderr, "%s: Host %s desconhecido ou inválido\n", argv[0], host);
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	client.sin_addr.s_addr = INADDR_ANY;
	client.sin_family = AF_INET;
	client.sin_port = 0;

	// Abertura ativa
	if((s = socket(AF_INET, SOCK_DGRAM , 0)) < 0) {
		fprintf(stderr, "%s: socket: ", argv[0]);
		perror("");
		exit(1);
	}

	if(bind(s, (struct sockaddr*) &client, sizeof(client)) < 0) {
		fprintf(stderr, "%s: bind: ", argv[0]);
		perror("");
		close(s);
		exit(1);
	}

	// Laço principal: obtém e envia linhas de texto
	while(fgets(buf, sizeof(buf), stdin)) {
		buf[MAX_LINE-1] = 0;
		len = strlen(buf) + 1;
		sendto(s, buf, len, 0, (struct sockaddr*) &server, sizeof(server));
	}

	close(s);
}
