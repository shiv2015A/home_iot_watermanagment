# Smart Home Automation and Water Leakage Detection System

## Overview

This project was developed during Class 8 as an exploration of embedded systems, IoT, and real-world automation. The system combines two independent subsystems:

1. A motion-activated home automation system capable of controlling a real 220V AC household bulb through a relay module.
2. A water leakage detection system that monitors water flow using dual YF-S201 flow sensors and estimates water loss through comparative analysis.

The project demonstrates how sensors, microcontrollers, and basic data analysis can be integrated to solve practical problems related to energy efficiency and water conservation.

---

## Objectives

### Home Automation

* Automatically control a household light based on human presence.
* Reduce unnecessary power consumption.
* Demonstrate safe interfacing between low-voltage electronics and high-voltage appliances.

### Water Monitoring

* Measure real-time water flow rates.
* Calculate cumulative water volume.
* Detect potential pipeline leakage by comparing measurements from multiple locations.

---

## Hardware Used

### Control System

* Arduino Uno
* PIR Motion Sensor
* 1-Channel 5V Relay Module
* 220V AC Bulb
* Bulb Holder

### Water Monitoring System

* 2 × YF-S201 Water Flow Sensors

### General Components

* Breadboard
* Jumper Wires
* USB Power Supply

---

## System Architecture

### Home Automation Module

```text
PIR Motion Sensor
        │
        ▼
   Arduino Uno
        │
        ▼
   Relay Module
        │
        ▼
   220V AC Bulb
```

When movement is detected by the PIR sensor, the Arduino activates the relay module, which closes the AC circuit and powers the bulb.

---

### Water Leakage Detection Module

```text
YF-S201 (Inlet)
        │
        ▼
   Arduino Uno
        ▲
        │
YF-S201 (Outlet)

        ▼

 Flow Rate Calculation
 Volume Calculation
 Leak Estimation
```

The Arduino continuously monitors both flow sensors and compares their measurements to identify possible water loss between the two locations.

---

## Working Principle

### Motion-Based Lighting

The PIR sensor detects infrared radiation emitted by moving people.

When motion is detected:

1. PIR outputs a HIGH signal.
2. Arduino processes the signal.
3. Relay is activated.
4. Household bulb turns ON.

When no motion is detected:

1. PIR outputs a LOW signal.
2. Relay is deactivated.
3. Bulb turns OFF.

---

### Water Flow Measurement

The YF-S201 sensor contains a turbine and Hall-effect sensor.

As water flows through the sensor:

* The turbine rotates.
* Pulses are generated.
* Pulse frequency is proportional to flow rate.

The Arduino counts these pulses and converts them into:

### Instantaneous Flow Rate

```text
Flow Rate (L/min) = Frequency / 7.5
```

### Total Water Volume

```text
Volume (L) = Pulse Count / 450
```

---

## Leakage Detection Logic

Two sensors are installed at different points in the pipeline.

The system calculates:

```text
Water Loss = Volume In - Volume Out
```

If the difference exceeds a predefined threshold, the system flags a potential leak.

Example:

```text
Volume In  = 100 L
Volume Out = 95 L

Estimated Water Loss = 5 L
```

This indicates possible leakage between the two measurement points.

---

## Challenges Faced

During development, several challenges were encountered:

* Understanding relay operation and safe switching of AC loads.
* Converting raw pulse outputs into usable flow measurements.
* Calibrating flow sensor readings.
* Handling noise and inconsistencies in sensor measurements.
* Designing logic that could distinguish normal variation from actual leakage.

---

## Skills Developed

This project helped develop skills in:

* Arduino Programming
* Embedded Systems
* Sensor Integration
* Relay Control
* AC Load Switching
* Serial Communication
* Data Analysis
* Real-Time Monitoring
* Engineering Problem Solving
* Hardware Debugging

---

## Results

### Home Automation System

Successfully controlled a real 220V AC bulb using a PIR sensor, Arduino Uno, and relay module.

### Water Monitoring System

Successfully measured water flow rate and cumulative water volume while detecting discrepancies that could indicate pipeline leakage.

---

## Future Improvements

Possible future upgrades include:

* ESP32-based Wi-Fi connectivity
* Mobile application integration
* Cloud data storage
* Leak alert notifications
* Automatic water shutoff valve
* Dashboard visualization
* Machine learning-based anomaly detection

---

## Project Reflection

This was one of my earliest engineering projects and played a significant role in developing my interest in electronics, embedded systems, and problem solving. It demonstrated how software and hardware can work together to automate tasks and address real-world challenges such as energy efficiency and water conservation.
