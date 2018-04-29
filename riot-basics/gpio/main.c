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

#include "board.h"

#include "periph/gpio.h"

kernel_pid_t pid;
static char stack[THREAD_STACKSIZE_MAIN];

static void gpio_cb(void *arg)
{
    (void) arg;
    msg_t msg;

    msg_send(&msg, pid);
}

static void *thread_handler(void *arg)
{
    (void) arg;
    
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Toggling LED1");
        gpio_toggle(LED1_PIN);
    }

    return NULL;
}

int main(void)
{
    puts("Push button RIOT application");

    pid = thread_create(stack, sizeof(stack),
                        THREAD_PRIORITY_MAIN - 1,
                        0,
                        thread_handler,
                        NULL, "thread");
    
    /* initialize on-board LED1 */
    gpio_init(LED1_PIN, GPIO_OUT);

    /* initialize on-board user button */
    gpio_init_int(BTN_B1_PIN, GPIO_IN, GPIO_RISING, gpio_cb, NULL);
    return 0;
}
