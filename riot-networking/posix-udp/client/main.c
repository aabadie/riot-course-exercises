#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "net/af.h"
#include "net/protnum.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define CLIENT_MESSAGE "Hello Server"

int main(void)
{
    struct sockaddr_in6 dst;
    int s;
    dst.sin6_family = AF_INET6;

    /* parse destination address */

    /* create the socket */

    while (1) {
        /* send CLIENT_MESSAGE to the server */


        xtimer_sleep(1);
    }

    return 0;
}