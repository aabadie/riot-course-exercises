/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "msg.h"
#include "shell.h"

static kernel_pid_t pid;
static char stack[THREAD_STACKSIZE_MAIN];

typedef struct {
    char data[42];
    mutex_t lock;
} data_buffer_t;

static data_buffer_t data_buffer;

static void *thread_handler(void *arg)
{
    (void) arg;
    
    return NULL;
}

static int send_command(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 1;
    }

    return 0;
}

static const shell_command_t commands[] = {
    { "send", "send a message", send_command },
    { NULL, NULL, NULL}
};

int main(void)
{
    puts("RIOT application with thread safe IPC");

    pid = thread_create(stack, sizeof(stack),
                        THREAD_PRIORITY_MAIN - 1,
                        0,
                        thread_handler,
                        NULL, "my thread");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
