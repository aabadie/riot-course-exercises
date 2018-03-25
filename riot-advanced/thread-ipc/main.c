#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "msg.h"
#include "shell.h"

static kernel_pid_t pid;
static char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg)
{
    (void) arg;
    
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

static int send_command(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 1;
    }

    msg_t msg;
    msg.content.ptr = (void *)argv[1];
    msg_send(&msg, pid);

    return 0;
}

static const shell_command_t commands[] = {
    { "send", "send a message", send_command },
    { NULL, NULL, NULL}
};

int main(void)
{
    puts("RIOT application with a thread");

    pid = thread_create(stack, sizeof(stack),
                       THREAD_PRIORITY_MAIN - 1,
                       0,
                       thread_handler,
                       NULL, "my thread");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
