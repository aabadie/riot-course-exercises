/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <stdlib.h>

#include "led.h"
#include "board.h"
#include "shell.h"

static int led_toggle_handler(int argc, char** argv)
{
    if (argc < 2) {
        printf("%s <led num>\n", argv[0]);
        return 1;
    }

    uint8_t led_num = atoi(argv[1]);
    switch(led_num) {
        case 0:
            LED0_TOGGLE;
            break;
        case 1:
            LED1_TOGGLE;
            break;
        case 2:
            LED2_TOGGLE;
            break;
        case 3:
            LED3_TOGGLE;
            break;
        default:
            printf("Unsupported LED number '%d'\n", led_num);
            return 1;
    }

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "toggle", "toggle a led status", led_toggle_handler },
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("LED toggle shell application");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
