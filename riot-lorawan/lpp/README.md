# Integration with Cayenne LPP

## **Goal:**
Write an application that sends sensor data to the Cayenne LPP
 integration in TTN.

## Instructions
  - Reuse the application in `~/riot-course-exercises/riot-lorawan/sensor` to
    send data in the Cayenne LPP format. Use the `cayenne-lpp` package
  - Add the Cayenne integration to your TTN application under
    `Integrations> add integration`.
  - Load the Cayenne LPP payload format under
    `Payload formats> Payload format`. Check the received data in
    the TTN dashboard.
  - **Optional**: Display the data in the `mydevices.com` dashboard.
    Follow the `Set up your myDevices account` in
    `https://www.thethingsnetwork.org/docs/applications/cayenne/`

_TIP_: Reuse parts from `~/RIOT/tests/pkg_cayenne-lpp`

_TIP_: Check the available `cayenne-lpp` functions in
 `https://github.com/aabadie/cayenne-lpp/blob/master/cayenne_lpp.h`
