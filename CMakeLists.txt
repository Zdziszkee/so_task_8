cmake_minimum_required(VERSION 3.22)
project(so_task_8 C)

set(CMAKE_C_STANDARD 23)

add_executable(client.c server.c message_queue.c)
