#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "message_queue.h"

//
// Created by Kacper Kuchta on 6/4/23.
//


mqd_t server_message_queue_descriptor;
char *server_message_queue_name;
mqd_t client_message_queue_descriptor;

void cleanUp(void) {
    close_message_queue(client_message_queue_descriptor);
    close_message_queue(server_message_queue_descriptor);
    unlink_message_queue(server_message_queue_name);
    printf("You can close window safely\n");
}

void signalHandler(int signal) {
    printf("\nSIGINT handled\n");
    cleanUp();
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Wrong number of arguments expected 2, received %d", argc);
        exit(EXIT_FAILURE);
    }
    server_message_queue_name = argv[1];

    /* at the end of file */
    if (atexit(cleanUp) != 0) {
        perror("Atexit error\n");
        exit(EXIT_FAILURE);
    }

    /* in ^C case */
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("Signal error\n");
        exit(EXIT_FAILURE);
    }

    /* create server queue */
    server_message_queue_descriptor = create_message_queue(server_message_queue_name, O_CREAT | O_EXCL | O_RDONLY, 0666,
                                                           MAX_MSG, MSG_SIZE);

    /* get attributes form queue */
    struct mq_attr attribute = get_message_queue_attribute(server_message_queue_descriptor);
    printf("name: %s descriptor: %d flags: %ld max_messages: %ld message_size: %ld current_message: %ld\n",
           server_message_queue_name, server_message_queue_descriptor, attribute.mq_flags, attribute.mq_maxmsg,
           attribute.mq_msgsize,
           attribute.mq_curmsgs);


    /* communication loop */
    while (1) {

        /* receive request */
        char request[MSG_SIZE];
        receive_message(server_message_queue_descriptor, request, MSG_SIZE, NULL);

        /* take values from request */
        int client_pid, num_1, num_2;
        char operator;
        sscanf(request, "%d %d %c %d", &client_pid, &num_1, &operator, &num_2);//better way?

        printf("Received request form %d, request: %d %c %d\n", client_pid, num_1, operator, num_2);

        /* calculate */
        char message[MSG_SIZE];
        switch (operator) {
            case '+':
                sprintf(message, "%d", num_1 + num_2);
                break;
            case '-':
                sprintf(message, "%d", num_1 - num_2);
                break;
            case '*':
                sprintf(message, "%d", num_1 * num_2);
                break;
            case '/':
                if (num_2 == 0) {
                    sprintf(message, "You can't divide by 0");
                    break;
                }
                sprintf(message, "%d", num_1 / num_2);
                break;
            default:
                sprintf(message, "Incorrect operator.");
                break;
        }

        /* open client queue */
        sprintf(request, "/%d", client_pid);
        client_message_queue_descriptor = open_message_queue(request, O_WRONLY);

        /* answer */
        printf("Sending message %s to process: %d\n\n", message, client_pid);
        send_message(client_message_queue_descriptor, message, MSG_SIZE, 1);
        close_message_queue(client_message_queue_descriptor);
    }
}
