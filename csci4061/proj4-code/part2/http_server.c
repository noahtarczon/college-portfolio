#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "connection_queue.h"
#include "http.h"

#define BUFSIZE 512
#define LISTEN_QUEUE_LEN 5
#define N_THREADS 5

const char *serve_dir;
int keep_going = 1;

void handle_sigint(int signo) {
    keep_going = 0;
}

int active_threads = 0;

//new thread function to handle communication
void *worker_thread_func(void *arg) {
    connection_queue_t *queue = (connection_queue_t *) arg;
    
    int fd;
    char resource_name[BUFSIZE];
    memset(resource_name, 0, BUFSIZE);
    char dir[BUFSIZE];
    strcpy(dir, queue->directory);
    char var[BUFSIZE];
    memset(var, 0, BUFSIZE);

    
    while (queue->shutdown != 1 && keep_going == 1) {
        if ((fd = connection_dequeue(queue)) == -1) {
            printf("worker thread error, need to return something??\n");
        }

        if (queue->shutdown != 1) {
        if(read_http_request(fd, resource_name) == -1){
            fprintf(stderr, "read http request\n");
            close(fd);
            continue;
        }

        strcat(dir, resource_name);

        if(write_http_response(fd, dir) == -1){
            fprintf(stderr, "write http response\n");
            close(fd);
            continue;
        }
        strcpy(dir, queue->directory);
        }
    }
    return NULL;
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

    //queue initialization
    connection_queue_t queue;
    if (connection_queue_init(&queue) == -1) {
        perror("connection queue init");
        return 1;
    }
    strcpy(queue.directory, server_dir);

    sigset_t newmask;
    sigset_t oldmask;
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);           //need to block all signals before thread creation then allow after
    int result;
    pthread_t worker_threads[N_THREADS];
    for (int i = 0; i < N_THREADS; i++) {
        if ((result = pthread_create(worker_threads + i, NULL, worker_thread_func, &queue)) != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(result));
            return 1;
        }
    }
    sigprocmask(SIG_SETMASK, &oldmask, NULL);           //allowing SIGs after thread creation

    while(keep_going != 0){
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

        if (fd_client > 0) {
            //enqueue new client
            if (connection_enqueue(&queue, fd_client) == -1) {
                perror("connection_enqueue");
                return 1;
            }
        }
    }


    if (connection_queue_shutdown(&queue) == -1) {
        perror("connection_queue_shutdown");
        return 1;
    }

    if(close(fd_socket)==-1){
        perror("close");
    }
    //Wait for all threads to complete
    for (int i = 0; i < N_THREADS; i++) {
        if ((result = pthread_join(worker_threads[i], NULL)) != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(result));
            
        }
    }
    
    if (connection_queue_free(&queue) < 0) {
        perror("connection_queue_free");
        return 1;
    }

    return 0;
}




