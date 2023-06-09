//
// Created by Kacper Kuchta on 6/4/23.
//

#ifndef SO_TASK_8_MESSAGE_QUEUE_H
#define SO_TASK_8_MESSAGE_QUEUE_H

#include <mqueue.h>
#include <stddef.h>

#define MAX_MSG 10
#define MSG_SIZE 30

/* function for creating message queues, returns message queue's descriptor */
mqd_t create_message_queue(const char *name, int oflag, mode_t mode, long max_msg, long msg_size);

/* function for opening exising message queue, returns message queue's descriptor */
mqd_t open_message_queue(const char *name, int flag);

/* function for closing message queue */
void close_message_queue(mqd_t mq_des);

/* function for unlinking message queue */
void unlink_message_queue(const char *name);

/* function for getting attributes, return struct */
struct mq_attr get_message_queue_attribute(mqd_t message_queue_descriptor);

/* function for sending messages through message queue */
void
send_message(mqd_t message_queue_descriptor, const char *message, size_t message_size, unsigned int message_priority);

/* function to receive messages through message queue */
void receive_message(mqd_t mq_des, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);

#endif //SO_TASK_8_MESSAGE_QUEUE_H
