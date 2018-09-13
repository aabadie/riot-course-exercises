# Adding Low Power

## Goal

Write an application that periodically sends data and sleeps for reducing
power consumption.

## Instructions

- Start from application [riot-lorawan/sensor](../sensor)
- Modify the sender thread so that it triggers a send only after an IPC
  message (`msg_t` type) is received
- After a LoRaWAN send, configure an RTC alarm 20s later
- In the RTC alarm callback, send a message to the sender thread
- Test the application
- Include `pm_layered.h` and use `pm_set(1)` to put the CPU in STOP mode after
  the RTC alarm is set (check the differences with `pm_set(0)`)

_TIP_: Check the RIOT RTC API in
 `http://doc.riot-os.org/group__drivers__periph__rtc.html`

_TIP_: Check the Layered PM infrastructure in
 `http://doc.riot-os.org/group__sys__pm__layered.html`
