#include <string.h>

#include "thread.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "hts221.h"
#include "hts221_params.h"

#include "cayenne_lpp.h"

#include "board.h"

#ifdef MODULE_PM_LAYERED
#include "pm_layered.h"
#endif
#include "periph/rtc.h"

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

/* Use the STOP mode to ensure memory retention between each send */
#define PM_MODE             (1)

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD              (20U)

/* Declare globally the loramac descriptor */
static semtech_loramac_t loramac;

/* Declare globally the sensor device descriptor */
static hts221_t hts221;

/* Cayenne LPP buffer */
static cayenne_lpp_t lpp;

/* Device and application informations required for OTAA activation */
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = { 0x00, 0xBD, 0x35, 0xDE, 0xE5, 0x8F, 0xF9, 0xCB };
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0xB0, 0x2F };
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = { 0xC9, 0x18, 0x70, 0x83, 0x5C, 0x73, 0xF5, 0xE3, 0x6E, 0xDF, 0x6E, 0x5D, 0xBD, 0x14, 0x45, 0xD0 };

static void rtc_cb(void *arg)
{
    (void) arg;
    msg_t msg;
    msg_send(&msg, sender_pid);
}

static void _prepare_next_alarm(void)
{
    struct tm time;
    rtc_get_time(&time);
    /* set initial alarm */
    time.tm_sec += PERIOD;
    while (time.tm_sec > 60) {
        time.tm_min++;
        time.tm_sec -= 60;
    }
    while (time.tm_min > 60) {
        time.tm_hour++;
        time.tm_min -= 60;
    }
    while (time.tm_hour > 24) {
        time.tm_mday++;
        time.tm_hour -= 24;
    }
    rtc_set_alarm(&time, rtc_cb, NULL);
}

static void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        /* waiting for messages before sending to TTN */
        msg_receive(&msg);

        /* do some measurements */
        uint16_t humidity = 0;
        int16_t temperature = 0;
        if (hts221_read_humidity(&hts221, &humidity) != HTS221_OK) {
            puts(" -- failed to read humidity!");
        }
        if (hts221_read_temperature(&hts221, &temperature) != HTS221_OK) {
            puts(" -- failed to read temperature!");
        }

        cayenne_lpp_add_temperature(&lpp, 0, (float)temperature / 10);
        cayenne_lpp_add_relative_humidity(&lpp, 1, (float)humidity / 10);

        printf("Sending LPP data\n");

        /* send the message every 20 seconds */
        semtech_loramac_send(&loramac, lpp.buffer, lpp.cursor);
        /* Wait until the send cycle has completed */
        semtech_loramac_recv(&loramac);

        /* clear buffer once done */
        cayenne_lpp_reset(&lpp);

        /* Schedule the next wake-up alarm */
        _prepare_next_alarm();

#ifdef MODULE_PM_LAYERED
        /* enable low-power mode */
        pm_set(PM_MODE);
#endif
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{
    if (hts221_init(&hts221, &hts221_params[0]) != HTS221_OK) {
        puts("Sensor initialization failed");
        LED3_TOGGLE;
        return 1;
    }
    if (hts221_power_on(&hts221) != HTS221_OK) {
        puts("Sensor initialization power on failed");
        LED3_TOGGLE;
        return 1;
    }
    if (hts221_set_rate(&hts221, hts221.p.rate) != HTS221_OK) {
        puts("Sensor continuous mode setup failed");
        LED3_TOGGLE;
        return 1;
    }

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

    /* trigger the first send */
    msg_t msg;
    msg_send(&msg, sender_pid);

    return 0; /* should never be reached */
}
