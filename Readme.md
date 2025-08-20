# TVC Flight Controller for Model Rocketry

## Brief overview
This repository contains the hardware, firmware and documentation for a Thrust Vector Control (TVC) flight controller designed for model rocketry. The system stabilizes and guides rockets using real-time sensor data and servo actuation.

## Hardware 
- **Microcontroller:** [Specify model, e.g., STM32F4]
- **Sensors:** IMU (Accelerometer, Gyroscope), Barometer
- **Actuators:** Digital Servos for TVC mechanism
- **Power:** LiPo battery
- **Other:** PCB, connectors, wiring

![Front-side](Hardware/FlightControllerV0.2/images/Front-side.png)

![Back-side](Hardware/FlightControllerV0.2/images/Front-side.png)

![Layout](Hardware/FlightControllerV0.2/images/PCB_LAYOUT.png)


## Firmware 
- **Sensor Fusion:** Reads IMU and barometer data
- **Control Algorithm:** PID/other for TVC actuation
- **Telemetry:** Serial/Radio communication for flight data
- **Failsafe:** Detects anomalies and triggers safe mode

## Flowcharts

## System Initialization

## References 