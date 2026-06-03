# IoT Home Automation and Water Leakage Detection System

## Overview

This project was developed during Class 8 as an introduction to embedded systems, IoT, and sensor-based automation. The system combines motion-activated home automation with a water leakage detection mechanism using multiple sensors and real-time data analysis.

The project demonstrates how low-cost microcontrollers and sensors can be used to automate tasks and monitor real-world systems.

---

## Features

### Smart Lighting System

* Uses a PIR (Passive Infrared) motion sensor to detect movement.
* Automatically activates an LED when motion is detected.
* Simulates a basic smart-home lighting system.

### Water Leakage Detection

* Uses two YF-S201 water flow sensors placed at different points in a water pipeline.
* Measures both:

  * Instantaneous flow rate (L/min)
  * Total water volume (Litres)
* Compares readings from both sensors to estimate water loss.
* Detects potential leaks when a significant difference is observed.

---

## Project Motivation

The goal of this project was to explore how sensors and microcontrollers can be used to solve practical real-world problems.

While home automation improves convenience and energy efficiency, water leakage detection helps reduce water wastage and identify infrastructure problems before they become serious.

---

## Hardware Used

* Arduino Uno
* PIR Motion Sensor
* 2 × YF-S201 Water Flow Sensors
* LED
* Breadboard
* Jumper Wires
* USB Connection

---

## How It Works

### Motion Detection Module

The PIR sensor continuously monitors the surrounding environment.

When movement is detected:

1. PIR sensor outputs a HIGH signal.
2. Arduino receives the signal.
3. LED turns ON.

When no movement is detected:

1. PIR sensor outputs a LOW signal.
2. Arduino turns the LED OFF.

---

### Water Monitoring Module

Two YF-S201 flow sensors are installed at different locations in the pipeline.

The sensors generate electrical pulses as water flows through them.

The Arduino:

1. Counts pulses from both sensors.
2. Converts pulse frequency into flow rate.
3. Calculates cumulative water volume.
4. Compares inlet and outlet measurements.
5. Estimates water loss between the two points.

If the difference exceeds a predefined threshold, the system flags a potential leak.

---

## Technical Concepts Used

* Arduino Programming
* Embedded Systems
* Sensor Integration
* Serial Communication
* Interrupt Handling
* Data Processing
* Flow Rate Measurement
* Basic IoT Design Principles

---

## Challenges

Some challenges encountered during development included:

* Understanding sensor calibration.
* Converting pulse data into meaningful flow measurements.
* Ensuring consistent readings from both flow sensors.
* Determining appropriate thresholds for leak detection.
* Debugging sensor communication and hardware connections.

---

## Skills Learned

Through this project, I learned:

* Basic electronics and circuit design.
* Sensor interfacing with Arduino.
* Real-time data collection and processing.
* Debugging hardware and software systems.
* Engineering problem-solving.
* Designing systems that interact with the physical world.

---

## Future Improvements

Possible future enhancements include:

* Wi-Fi connectivity using ESP8266 or ESP32.
* Cloud-based monitoring dashboard.
* Mobile notifications for leak alerts.
* Data logging and analytics.
* Automatic shutoff valve integration.
* Machine learning-based anomaly detection.

---

## Project Status

Completed as an educational project and serves as one of my earliest explorations into embedded systems, IoT, and engineering design.

---

## Author

Shivansh Dubey

Student | Technology Enthusiast | AI, Embedded Systems, Astronomy, and Engineering
