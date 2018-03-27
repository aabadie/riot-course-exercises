#include <string.h>

#include "thread.h"
#include "xtimer.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "board.h"

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

/* Declare globally the loramac descriptor */
static semtech_loramac_t loramac;

/* Device and application informations required for OTAA activation */
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x00, 0xBD, 0x35, 0xDE, 0xE5, 0x8F, 0xF9, 0xCB };
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0xB0, 0x2F };
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0xC9, 0x18, 0x70, 0x83, 0x5C, 0x73, 0xF5, 0xE3, 0x6E, 0xDF, 0x6E, 0x5D, 0xBD, 0x14, 0x45, 0xD0 };

/* The simple message to send */
const char *message = "This is RIOT!";

static void *sender(void *arg)
{
    (void)arg;

    while (1) {
        printf("Sending message: %s\n", message);
        /* send the message every 20 seconds */
        semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message));
        /* Wait until the send cycle has completed */
        if (semtech_loramac_recv(&loramac) == SEMTECH_LORAMAC_DATA_RECEIVED) {
            loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
            printf("Data received: %s\n", (char *)loramac.rx_data.payload);
            uint8_t count = 10;
            while (count--) {
                LED1_TOGGLE;
                xtimer_usleep(100 * US_PER_MS);
                LED1_TOGGLE;
                xtimer_usleep(100 * US_PER_MS);
            }
        }

        xtimer_sleep(20);
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{
    /* initialize the loramac stack */
    semtech_loramac_init(&loramac);

    /* use a fast datarate so we don't use the physical layer too much */
    semtech_loramac_set_dr(&loramac, 5);

    /* set the LoRaWAN keys */
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* start the OTAA join procedure */
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }

    puts("Join procedure succeeded");

    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "sender");

    return 0; /* should never be reached */
}
