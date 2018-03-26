#include <stdio.h>

#include "thread.h"
#include "xtimer.h"

static char stack1[THREAD_STACKSIZE_MAIN];

static void *thread1_handler(void *arg)
{
    (void) arg;
    
    return NULL;
}

int main(void)
{
    puts("Multi LED blinker application");

    thread_create(stack1, sizeof(stack1),
                  THREAD_PRIORITY_MAIN - 1,
                  0,
                  thread1_handler,
                  NULL, "thread1");

    return 0;
}
