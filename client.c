#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <libgen.h>


#define ERROR(x) error_at_line(-1, errno, __FILE__, __LINE__, x)
#define ERROR_H(x) error_at_line(-1, h_errno, __FILE__, __LINE__, x)


void usage()
{
    printf("Usage: %s angle hostname port\n", "./client");
}


int main(int argc, char * argv[])
{
    if (argc < 4) {
        usage();
        exit(1);
    }
    int angle = atoi(argv[1]);
    if (-90 > angle || angle > 90) {
        printf("Bad angle\n");
        exit(1);
    } 
    char * hostname = argv[2];
    int port = atoi(argv[3]);
    
    struct hostent *he = gethostbyname(hostname);
    if (he == NULL) {
        ERROR_H("gethostbyname");
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    memcpy(&(addr.sin_addr.s_addr), he->h_addr, he->h_length);
    addr.sin_port = htons(port);
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        ERROR("socket");
    }
    
    char message[10];
    snprintf(message, sizeof(message), "%d\n", angle);
    
    ssize_t n = sendto(sock, message, strlen(message), 0,
        (struct sockaddr *) &addr, sizeof(addr));
    if (n < 0) {
        ERROR("sendto");
    } else {
        printf("<- %s", message);
    }
    
    close(sock);
    
    return 0;
}


