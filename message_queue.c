//
// Created by Kacper Kuchta on 6/4/23.
//

#include "message_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>


mqd_t create_message_queue(const char *name, int flag, mode_t mode, struct mq_attr *attribute) {
    mqd_t message_queue_descriptor = mq_open(name, flag, mode, &attribute);
    if (message_queue_descriptor == -1) {
        perror("Failed to create queue\n");
        exit(EXIT_FAILURE);
    }
    return message_queue_descriptor;
}

mqd_t open_message_queue(const char *name, int flag) {
    mqd_t message_queue_descriptor = mq_open(name, flag);
    if (message_queue_descriptor == -1) {
        perror("Failed to open queue\n");
        exit(EXIT_FAILURE);
    }
    return message_queue_descriptor;
}

void send_message(mqd_t message_queue_descriptor, const char *message, size_t message_size, unsigned int message_priority) {
    int success = mq_send(message_queue_descriptor, message, message_size, message_priority);
    if (success == -1) {
        perror("Failed to send message\n");
        exit(EXIT_FAILURE);
    }
}

void receive_message(mqd_t message_queue_descriptor, char *message, size_t message_size, unsigned int *message_priority) {
    ssize_t success = mq_receive(message_queue_descriptor, message, message_size, message_priority);
    if (success == -1) {
        perror("Failed to receive message\n");
        exit(EXIT_FAILURE);
    }
}

void close_message_queue(mqd_t message_queue_descriptor) {
    int success = mq_close(message_queue_descriptor);
    if (success == -1) {
        perror("Failed to close queue\n");
        exit(EXIT_FAILURE);
    }
}

void unlink_message_queue(const char *name) {
    int success = mq_unlink(name);
    if (success == -1) {
        perror("Failed to unlink queue\n");
        exit(EXIT_FAILURE);
    }
}

struct mq_attr get_message_queue_attribute(mqd_t message_queue_descriptor) {
    struct mq_attr attribute;
    int success = mq_getattr(message_queue_descriptor, &attribute);
    if (success == -1) {
        perror("Failed to get attributes\n");
        exit(EXIT_FAILURE);
    }
    return attribute;
}