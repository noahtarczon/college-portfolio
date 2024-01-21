#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "http.h"

#define BUFSIZE 512

const char *get_mime_type(const char *file_extension) {
    if (strcmp(".txt", file_extension) == 0) {
        return "text/plain";
    } else if (strcmp(".html", file_extension) == 0) {
        return "text/html";
    } else if (strcmp(".jpg", file_extension) == 0) {
        return "image/jpeg";
    } else if (strcmp(".png", file_extension) == 0) {
        return "image/png";
    } else if (strcmp(".pdf", file_extension) == 0) {
        return "application/pdf";
    }

    return NULL;
}

int read_http_request(int fd, char *resource_name) {
    // TODO Not yet implemented
    int sock_fd = dup(fd);
    if(sock_fd == -1){
        perror("dup");
        return -1;
    }
    FILE *socket_stream = fdopen(sock_fd, "r");
    if(socket_stream == NULL){
        perror("fdopen");
        close(sock_fd);
        return -1;
    }
    if(setvbuf(socket_stream, NULL, _IONBF, 0) != 0){
        perror("setvbuf");
        fclose(socket_stream);
        return -1;
    }
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    if(fgets(buf, BUFSIZE, socket_stream) == NULL){
        perror("fgets");
        return -1;
       
    }
    char *token = strtok(buf, " ");
    // error check
    token = strtok(NULL, " ");
    if(token == NULL){
        perror("strtok");
        return -1;
    }
    if(strcpy(resource_name, token) == NULL){
        perror("strncpy");
        return -1;
    }
    while(fgets(buf, BUFSIZE, socket_stream) != NULL){
        if (strcmp(buf, "\r\n") == 0) {
            break;
        }
    }
    
    if(fclose(socket_stream)!= 0){
        perror("fclose");
        return -1;
    }

    return 0;
}

int write_http_response(int fd, const char *resource_path) {
  
    struct stat s_buf;
    if(stat(resource_path, &s_buf) == -1){
        if(errno == ENOENT){
            char *message = "HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            if(write(fd, message, strlen(message))==-1){
                perror("write 404");
                return -1;
            }
            return -1;
        }
        else{
            perror("stat");
            return -1;
        }
    }

    char *end = strrchr(resource_path, '.');
    if(end == NULL){
        perror("strrchr");
        return -1;
    }

    //t is the file type
    const char *t = get_mime_type(end);
    if(t == NULL){
        perror("get_mine_type");
        return -1;
    }

    long int var = s_buf.st_size;
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
   
    snprintf(buf,BUFSIZE,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", t, var);

    if(write(fd, buf, strlen(buf)) == -1){
        perror("write");
        return -1;
    }
    
    //open returns the file descriptor of the open file
    int new_fd = open(resource_path, O_RDONLY);
    if(new_fd == -1){
        perror("open");
        return -1;
    }
    int num = 0;
    int total = 0;
    while(total < var){
        memset(buf, 0, BUFSIZE);
        //read from the resource path
        if((num = read(new_fd, &buf, BUFSIZE))==-1){
            close(new_fd);
            perror("read");
            return -1;
        }
        //write to the fd given (client socket)
        if(write(fd, buf, BUFSIZE) == -1){
            close(fd);
            perror("write");
            return -1;
        }
        total += num;
    }
    if(close(new_fd) == -1){
        perror("close");
        return -1;
    }
    return 0;
}

