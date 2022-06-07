# SmartEVSE WiFi Controller
An add-on for the SmartEVSE (v2) to provide wireless control of the charging modes.

## Hardware: ESP8266
The hardware used is any generic ESP8266 board. I started on a NodeMCUv2 board (`nodemcuv2`), then moved to a `modwifi` board I had lying around.

## SmartEVSE <-> ESP connection
The only pins needed are a single UART pair (RX + TX) + GND + 3V supply. The UART pins have to be connected to the SmartEVSE's "FTDI" connector.
You can also power your ESP8266 board from the 3V supply on that same connector!

## ESPhome magic!
ESPhome is such a powerful project. Instead of firing up an embedded project from scratch to target the ESP8266 (e.g. using ESP-IDK, or even using PlatformIO),
you just write a few lines of yaml and run a couple of Python commands.
In less than an hour you have a custom firmware with FOTA and wireless logging. Amazing!

What is especially powerful, is that you can continue to develop or refine your application, while the board is already installed and this can all be done wirelessly.
Recompiling and flashing a new firmware is as simple as `esphome run smartevse-wifi-controller.yaml`
