#include <stdio.h>

#include "thread.h"
#include "shell.h"

static char stack[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg)
{
    (void) arg;
    puts("Hello from thread");
    
    while (1) {}

    return NULL;
}

int main(void)
{
    puts("First RIOT application with a thread");

    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN,
                  0,
                  thread_handler,
                  NULL, "my thread");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
