# Smart Home Security Alert System

## Project Overview

The Smart Home Security Alert System is an IoT-based security project designed to detect unauthorized movement and provide an immediate alert.

The system uses an ESP32-CAM, PIR motion sensor, buzzer, LED, and Wi-Fi connectivity.

## Features

- Motion detection using a PIR sensor
- Image capture using ESP32-CAM
- Buzzer alert when motion is detected
- LED indication during an alert
- Wireless security notification
- Camera-based monitoring

## Components Used

- ESP32-CAM
- PIR Motion Sensor
- Buzzer
- LED
- Resistor
- Jumper Wires
- 5V Power Supply
- Wi-Fi

## Working Principle

1. The PIR sensor continuously monitors for movement.
2. When motion is detected, the ESP32-CAM activates the buzzer and LED.
3. The ESP32-CAM captures an image.
4. The security alert can be sent to a mobile device through the configured communication service.
5. The system returns to monitoring mode after the alert.

## Circuit Diagram

The proposed circuit connection diagram is provided in `circuit_diagram.png`.

## Project Status

**Prototype / Design Stage**

The circuit diagram and software have been prepared as part of the project design. Physical hardware implementation and testing are planned as the next stage.

## Future Improvements

- Cloud-based image storage
- Real-time mobile monitoring
- Multiple sensors
- Intruder detection using AI
- Remote control through a mobile application

## Author

**Teja Sree Yeddula**

B.Tech – Electronics and Communication Engineering
