#include <stdio.h>

#include "shell.h"

static int cmd_handler(int argc, char** argv)
{
    if (argc < 3) {
        printf("usage: %s <arg1> <arg2>\n", argv[0]);
        return 1;
    }

    printf("Using arguments %s and %s\n", argv[1], argv[2]);
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "command", "description", cmd_handler },
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("RIOT application with shell");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
