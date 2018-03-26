#include <stdio.h>
#include <string.h>
#include <time.h>

#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#include "board.h"

kernel_pid_t pid;
static char stack[THREAD_STACKSIZE_MAIN];

static void *blink_thread(void *arg)
{
    (void) arg;

    return NULL;
}

int main(void)
{
    puts("RTC alarm RIOT application");

    pid = thread_create(stack, sizeof(stack),
                        THREAD_PRIORITY_MAIN - 1,
                        0,
                        blink_thread,
                        NULL, "thread");

    return 0;
}
