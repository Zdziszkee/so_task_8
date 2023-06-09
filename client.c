//
// Created by Kacper Kuchta on 6/4/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include "message_queue.h"

mqd_t server_message_queue_descriptor, client_message_queue_descriptor;
char *server_message_queue_name;
char client_message_queue_name[32];

void cleanUp(void) {
    close_message_queue(server_message_queue_descriptor);
    close_message_queue(client_message_queue_descriptor);
    unlink_message_queue(client_message_queue_name);
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

    /* at the end of program */
    if (atexit(cleanUp) != 0) {
        perror("atexit error");
        _exit(1);
    }

    /* in ^c case */
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("Błąd signal");
        exit(EXIT_FAILURE);
    }



    /* open server queue */
    server_message_queue_descriptor = open_message_queue(server_message_queue_name, O_WRONLY);

    /* get attributes form queue */
    struct mq_attr attribute = get_message_queue_attribute(server_message_queue_descriptor);
    printf("name: %s descriptor: %d flags: %ld max_messages: %ld message_size: %ld current_message: %ld\n",
           server_message_queue_name, server_message_queue_descriptor, attribute.mq_flags, attribute.mq_maxmsg, attribute.mq_msgsize,
           attribute.mq_curmsgs);

    /* create queue name */
    pid_t process_id = getpid();
    sprintf(client_message_queue_name, "/%d", process_id);
    /* create client queue */

    printf("You can only use [+, -, *, /]\n");

    while (1) {

        printf("\nType operation:\n");
        char message[MSG_SIZE - 10];
        if (scanf("%s", message) == EOF) {
            printf("End of file\n");
            exit(EXIT_SUCCESS);
        }

        /* add PID to request */
        char request[MSG_SIZE];
        sprintf(request, "%d %s", process_id, message);

        /* send request to server */
        send_message(server_message_queue_descriptor, request, MSG_SIZE, 0);
        printf("\nSent request: %s\n", request);


        /* receive answer from server */
        client_message_queue_descriptor = create_message_queue(client_message_queue_name, O_CREAT | O_EXCL | O_RDONLY,0666, MAX_MSG,MSG_SIZE);
        receive_message(client_message_queue_descriptor, request, MSG_SIZE, NULL);
        printf("Answer from server: %s\n\n", request);

        /* close client queue */
        close_message_queue(client_message_queue_descriptor);
        /* unlink client queue */
        unlink_message_queue(client_message_queue_name);
    }
    return 0;
}

