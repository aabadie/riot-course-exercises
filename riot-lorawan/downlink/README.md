# Downlinks from the LoRaWAN network

## **Goal**
Write an application that blinks a LED when it receives a message from the
LoRaWAN network

## Instructions
  - Reuse ~/riot-course-exercises/riot-lorawan/lpp` to make LED1 blink
    when receiving a message from the LoRaWAN network.
  - Send downlink messages to your node from the TTN network: go to
    `Devices> [your_device] > Downlinks`
  - **Optional**: Use the TTN MQTT broker to send and receive data from
    your node. Check the appendix below.

## Appendix: Using the MQTT API
  - The Mosquitto MQTT Client is used.
  - Listening to uplink messages (device to network):
```
$ mosquitto_sub -h eu.thethings.network -p 1883 -u <username> -P <password>
-t '+/devices/+/up'
```
  - Sending a downlink message (network to device):
```
$ mosquitto_pub -h eu.thethings.network -p 1883 -u <username> -P <password>
-t '<application id>/devices/<device id>/down'
-m '{"port":2, "payload_raw":"dGVzdA=="}'
```

_TIP_: base64 payload can be decoded with the command `$ base64 -d <<< dGVzdA==`
