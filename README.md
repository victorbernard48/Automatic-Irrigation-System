
# Automatic Irrigation system for Apartment Balconies

Project Description: Automated Balcony Plant Irrigation System

This project implements an automated plant irrigation system designed specifically for balcony gardens. It leverages the power of the ESP32 microcontroller, coupled with a DHT22 sensor for environmental monitoring and a solar pump for watering plants. The system is orchestrated by Mosquitto MQTT and Node-RED, hosted on a Raspberry Pi acting as the central automation hub.



## Overview
This project aims to automate home irrigation using an ESP32 board equipped with a DHT22 sensor and a solar pump. The automation is facilitated through Mosquitto MQTT and Node-RED installed on a Raspberry Pi acting as the automation hub. The ESP32 board is installed in the balcony to automatically water plants based on environmental conditions and user-defined schedules.
## Schematic
![alt text]([https://raw.githubusercontent.com/abdulmoizshaikh/recipe-book/master/src/assets/images/Screenshot%20from%202021-03-31%2011-08-15.png?token=AGEPYAA3FGL4MHI7LZA75VLAMQJ5A](https://github.com/victorbernard48/Automatic-Irrigation-System/blob/main/Automatic%20Balcony%20Plants%20irrigation%20System.drawio%20(2).png))
## Hardware
1. Hardware Requirements:
2. ESP32 board
3. DHT22 sensor
4. Solar pump
5. Raspberry Pi (as the automation hub)
## Software Requirements
1. Mosquitto MQTT broker
2. Node-RED
3. Arduino IDE (for ESP32 programming)
## Getting started
1. Clone the project repository from GitHub.
2. Install the necessary libraries in the Arduino IDE: WiFi,
3. PubSubClient, Wire, Adafruit_Sensor, DHT.
4. Create a credentials.h file and define your WiFi SSID and password.
5. Adjust the MQTT server IP address (mqtt_server) in the code to match your setup.
6. Connect the hardware components according to the circuit diagram.
7. Upload the Arduino sketch (ESP32_Home_Automation.ino) to the ESP32 board.
## Usage
1. Setting Up MQTT and Node-RED
2. Install and configure Mosquitto MQTT broker on the Raspberry Pi.
3. Install Node-RED on the Raspberry Pi.
4. Import the provided Node-RED flow for controlling the irrigation system.
## Controlling the system
Access the Node-RED dashboard to monitor sensor data and control the irrigation system.
Use MQTT topics to send commands to the ESP32 board:
esp32/output: Control the system's output (e.g., turn on/off LEDs).
esp32/pump/morninghour, esp32/pump/morningmin: Set the morning irrigation time.
esp32/pump/eveninghour, esp32/pump/eveningmin: Set the evening irrigation time.
esp32/pump/pumpontime: Set the pump's ON time.
esp32/pump/manualcontrol: Manually control the pump.
## Automated Irrigation
1. Define the desired morning and evening irrigation times through the Node-RED dashboard.
2. Set the pump's ON time according to your plant watering needs.
3. Ensure the ESP32 board is connected to the MQTT broker to receive commands.
## Notes

Ensure the Raspberry Pi and ESP32 board are connected to the same local network.
Adjust the code and Node-RED flow as needed to fit your specific requirements.
Monitor system performance and adjust settings as necessary for optimal plant care.
For more details, refer to the project repository.
## Documentation

[Documentation](https://github.com/victorbernard48/Automatic-Irrigation-System/edit/main/README.md)

