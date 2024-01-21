#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http.h"


#define BUFSIZE 512
#define LISTEN_QUEUE_LEN 5


int keep_going = 1;

void handle_sigint(int signo) {
    keep_going = 0;
}


int main(int argc, char **argv) {
    // First command is directory to serve, second command is port
    if (argc != 3) {
        printf("Usage: %s <directory> <port>\n", argv[0]);
        return 1;
    }
    // Uncomment the lines below to use these definitions:
    const char *server_dir = argv[1];
    const char *port = argv[2];

    struct sigaction sigact;
    sigact.sa_handler = handle_sigint;
    sigfillset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    if(sigaction(SIGINT, &sigact, NULL)==-1){
        perror("sigaction");
        return 1;
    }

    // Setup the socket
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *s;

    int val = getaddrinfo(NULL, port, &hints, &s);
    if(val != 0){
        fprintf(stderr, "getaddrinfo fail: %s\n", gai_strerror(val));
        return 1;
    }

    int fd_socket = socket(s->ai_family, s->ai_socktype, s->ai_protocol);
    if(fd_socket == -1){
        perror("socket");
        freeaddrinfo(s);
        return 1;
    }

    int x = bind(fd_socket, s->ai_addr, s->ai_addrlen);
    if(x==-1){
        perror("bind");
        freeaddrinfo(s);
        close(fd_socket);
        return 1;
    }
    
    freeaddrinfo(s);
    int y = listen(fd_socket, LISTEN_QUEUE_LEN);
    if(y == -1){
        perror("listen");
        close(fd_socket);
        return 1;
    }

    char resource_name[BUFSIZE];
    memset(resource_name, 0, BUFSIZE);
    char var[BUFSIZE];
    memset(var, 0, BUFSIZE);

    while(keep_going != 0){
        printf("Waiting for client to connect\n");
        int fd_client = accept(fd_socket, NULL, NULL);
        if(fd_client == -1){
            if(errno != EINTR){
                perror("accept");
                close(fd_socket);
                return 1;
            }
            else{
                break;
            }
        }
        if(read_http_request(fd_client, resource_name) == -1){
            fprintf(stderr, "read http request\n");
            close(fd_client);
            continue;
        }

        snprintf(var, BUFSIZE, "%s%s", server_dir, resource_name);

        if(write_http_response(fd_client, var) == -1){
            fprintf(stderr, "write http response\n");
            close(fd_client);
            continue;
        }
        if(close(fd_client)==-1){
            perror("close");
            continue;
        }
    }
    if(close(fd_socket)==-1){
        perror("close");
    }
    return 0;
}




