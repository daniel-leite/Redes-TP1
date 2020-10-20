#ifndef UDP_SOCK_H
#define UDP_SOCK_H

#define BUF_MAX 256

void sockinit(char* destaddr, char* myaddr, int destport, int myport);
void sockclose();
void socksend(char* buf, int len);
int sockrecv(char* buf);
unsigned int sockrtt(char* buf, int len, int reps);

#endif // UDP_SOCK_H
