#include <stdio.h>
#include <string.h>
#include <time.h>

#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#include "board.h"

#include "periph/rtc.h"

kernel_pid_t pid;
static char stack[THREAD_STACKSIZE_MAIN];

static void rtc_alarm_cb(void *arg)
{
    (void) arg;
    msg_t msg;

    msg_send(&msg, pid);
}

static void *blink_thread(void *arg)
{
    (void) arg;

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Toggling LED1");
        LED1_TOGGLE;
        xtimer_sleep(1);
        LED1_TOGGLE;

        struct tm time;
        rtc_get_time(&time);
        printf("Current time: %d:%d:%d\n",
               time.tm_hour, time.tm_min, time.tm_sec);
        time.tm_sec += 5;

        rtc_set_alarm(&time, rtc_alarm_cb, NULL);
    }

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

    struct tm time;
    rtc_get_time(&time);
    printf("Startup time: %d:%d:%d\n",
           time.tm_hour, time.tm_min, time.tm_sec);

    msg_t msg;
    msg_send(&msg, pid);

    return 0;
}
