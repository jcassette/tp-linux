#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#define ERROR(x) error_at_line(-1, errno, __FILE__, __LINE__, x)
#define ERROR_H(x) error_at_line(-1, h_errno, __FILE__, __LINE__, x)


void usage()
{
    printf("Usage: %s port\n", "./server");
}


int main(int argc, char * argv[])
{
    if (argc < 2) {
        usage();
        exit(1);
    }
    int port = atoi(argv[1]);
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        ERROR("socket");
    }
    
    struct sockaddr_in localaddr;
    memset(&localaddr, 0, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = INADDR_ANY;
    localaddr.sin_port = htons(port);
    
    if (bind(sock, (struct sockaddr *) &localaddr, sizeof(localaddr))) {
       ERROR("bind");
    }
    
    printf("Ready\n");
    for (;;) {
        char buffer[10];
        struct sockaddr_in addr;
        socklen_t addrlen;
        ssize_t n = recvfrom(sock, buffer, sizeof(buffer), 0,
            (struct sockaddr *) &addr, &addrlen);
        if (n < 0) {
            ERROR("recvfrom");
        } else {
            printf("-> %s", buffer);
        }
    }
    
    close(sock);
    return 0;
}

