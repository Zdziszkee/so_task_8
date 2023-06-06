#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Wrong number of arguments expected 3, received %d", argc);
        exit(EXIT_FAILURE);
    }
    char *server_message_queue_name = argv[1];
    long client_number = strtol(argv[2], NULL, 10);

    //create server process
    switch (fork()) {
        case -1:
            perror("Fork error\n");
            exit(EXIT_FAILURE);

        case 0:
            if (execlp("./server.x", "./server.x", server_message_queue_name, (char *) NULL) == -1) {
                perror("Execlp error\n");
                exit(EXIT_FAILURE);
            }
            break;

        default:
            break;
    }

    //create client processes
    for (int i = 0; i < client_number; ++i) {
        switch (fork()) {
            case -1:
                perror("Fork error\n");
                exit(EXIT_FAILURE);

            case 0:
                if (execlp("./client.x", "./client.x", server_message_queue_name, (char *) NULL) == -1) {
                    perror("Execlp error\n");
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                break;
        }
    }
    return 0;
}
