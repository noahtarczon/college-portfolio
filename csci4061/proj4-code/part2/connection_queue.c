#include <stdio.h>
#include <string.h>
#include "connection_queue.h"

int connection_queue_init(connection_queue_t *queue) {
    int ret_val = 0;
    queue -> read_idx = 0;
    queue -> write_idx = 0;
    queue -> shutdown = 0;
    queue->length = 0;
    if(pthread_mutex_init(&queue->lock, NULL) !=0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1; 

    }  
    if((ret_val = pthread_cond_init(&queue->full, NULL)) != 0){ 
        fprintf(stderr, "pthread error\n");
        pthread_mutex_destroy(&queue->lock);
        return -1;

    }
    if((ret_val = pthread_cond_init(&queue->empty, NULL)) != 0){
         fprintf(stderr, "pthread error\n");
         pthread_mutex_destroy(&queue->lock);
         pthread_cond_destroy(&queue->full);
         return -1; 
    }
    return 0;
}

int connection_enqueue(connection_queue_t *queue, int connection_fd) {
    int ret_val;
    if((ret_val = pthread_mutex_lock(&queue->lock)) !=0){
        fprintf(stderr, "pthread_lock in enqueue: %s\n", strerror(ret_val));
        return -1;
    }  
    while (queue->length == CAPACITY && queue->shutdown == 0) {
       if((ret_val = pthread_cond_wait(&queue->full, &queue->lock))!= 0){
            fprintf(stderr, "pthread_wait in enqueue: %s\n", strerror(ret_val));
            pthread_mutex_unlock(&queue->lock);
            return -1;
       }
    }

    if (queue->shutdown != 0) {
        if((ret_val = pthread_mutex_unlock(&queue->lock)) !=0){
        fprintf(stderr, "pthread_unlock in enqueue shutdown: %s\n", strerror(ret_val));
        return -1;
    }
    }
    queue->client_fds[queue->write_idx] = connection_fd;
    queue->write_idx = (queue->write_idx + 1) % CAPACITY;     
    queue->length++; 
    if((ret_val = pthread_cond_signal(&queue->empty)) !=0){
        fprintf(stderr, "pthread_signal in enqueue: %s\n", strerror(ret_val));
        pthread_mutex_unlock(&queue->lock);
        return -1;
    }
    if((ret_val = pthread_mutex_unlock(&queue->lock)) !=0){
        fprintf(stderr, "pthread_unlock in enqueue: %s\n", strerror(ret_val));
        return -1;
    }
    return 0;
}


int connection_dequeue(connection_queue_t *queue) {
    int ret_val;
    if((ret_val = pthread_mutex_lock(&queue->lock)) != 0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1;
    }
    while(queue->length == 0 && queue->shutdown == 0){
        if((ret_val = pthread_cond_wait(&queue->empty, &queue->lock)) != 0){
            fprintf(stderr, "pthread: %s\n", strerror(ret_val));
            pthread_mutex_unlock(&queue->lock);
            return -1;
        }
    }
    if(queue->shutdown != 0){
        pthread_mutex_unlock(&queue->lock);
        return 0;
    }
    int connection_fd = queue->client_fds[queue->read_idx];
    queue->read_idx = (queue -> read_idx + 1) % CAPACITY;
    queue->length--;

    if((ret_val = pthread_cond_signal(&queue->full)) != 0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1;
    }
    if((ret_val = pthread_mutex_unlock(&queue->lock) != 0)){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1;
    }
    return connection_fd;
}



int connection_queue_shutdown(connection_queue_t *queue) {
    int ret_val;
    
    if((ret_val = pthread_mutex_lock(&queue->lock))!=0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1;
    }
    queue->shutdown = 1;

    if((ret_val = pthread_cond_broadcast(&queue->full))!=0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        pthread_mutex_unlock(&queue->lock);
        return -1;
    }
    if((ret_val = pthread_cond_broadcast(&queue->empty))!=0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1;
    }
    if(pthread_mutex_unlock(&queue->lock) != 0){
        fprintf(stderr, "pthread: %s\n", strerror(ret_val));
        return -1;
    }
    return 0;
}

int connection_queue_free(connection_queue_t *queue) {
    // use destroy here for pthread_mutex_destroy and all the others
    int ret_val;
    if((ret_val = pthread_mutex_destroy(&queue->lock))!=0){
        fprintf(stderr, "pthread_mutex_destroy: %s\n", strerror(ret_val));
        return -1;
    }
    if((ret_val = pthread_cond_destroy(&queue->full))!= 0){
        fprintf(stderr, "pthread_cond_destroy: %s\n", strerror(ret_val));
        return -1;
    }
    if((ret_val = pthread_cond_destroy(&queue->empty))!=0){
        fprintf(stderr, "pthread_cond_destroy: %s\n", strerror(ret_val));
        return -1;
    }
    return 0;
}
