# Hovercraft-Team1-F2026

# ENGR 290 — Autonomous Hovercraft | Team 1

**Concordia University — Fall 2026**

## 1. Project Overview

This project involves the design, development, implementation and testing of an autonomous hovercraft for the ENGR 290 Engineering Design course at Concordia University.

The objective is to develop a lightweight, efficient and fully autonomous hovercraft capable of navigating a predefined obstacle course without human intervention.

Our selected design, **Design 1 — Compact Rounded-D**, combines embedded systems programming, sensor integration, fluid mechanics, mechanical design and autonomous navigation.

The control system is implemented in **AVR C using register-level programming on the ATmega328P microcontroller**, minimizing dependency on external Arduino libraries.

## 2. Project Objectives

- Develop a stable and efficient air-cushion system.
- Navigate the course autonomously.
- Detect approaching walls and execute controlled turns.
- Detect the overhead bar and perform the required stopping manoeuvre.
- Identify the exit opening and navigate through it.
- Maintain heading stability using an IMU.
- Minimize component count, weight and power consumption.
- Optimize course completion time and competition score.
- Develop modular, maintainable and reusable embedded software.

## 3. Selected Design

### Design 1 — Compact Rounded-D

The Compact Rounded-D design was selected to improve manoeuvrability, reduce the turning envelope and provide sufficient clearance within the competition lanes.

The hovercraft uses two independent fans:

- **Lift fan:** Generates the air cushion beneath the hovercraft.
- **Thrust fan:** Generates propulsion.
- **Steering servo:** Redirects thrust to control the hovercraft's direction.

Separating lift and propulsion allows the hovercraft to maintain its air cushion while stopping or adjusting its forward motion.

## 4. Hardware Components

| Component | Quantity | Function |
|---|---:|---|
| ATmega328P | 1 | Main microcontroller |
| Sharp GP2Y0A21YK0F IR sensor | 1 | Side-wall and exit detection |
| HC-SR04 ultrasonic sensor | 1 | Overhead-bar detection |
| HC-SR04 ultrasonic sensor | 1 | Front-wall detection |
| MPU6050 IMU | 1 | Yaw measurement and heading control |
| Servo motor | 1 | Thrust-vector steering |
| Lift fan | 1 | Air-cushion generation |
| Thrust fan | 1 | Forward propulsion |
| Motor drivers | As required | Fan power and speed control |
| Battery | 1 | System power supply |
| Hovercraft chassis and skirt | 1 | Mechanical structure and air containment |

### Sensor Configuration

The selected configuration consists of:

**2 Ultrasonic Sensors + 1 IR Sensor + 1 IMU**

This configuration provides dedicated sensing for front-wall detection, overhead-bar detection, exit detection and heading estimation.

## 5. System Architecture

The system is divided into three main layers.

### Input Layer

- Front ultrasonic sensor
- Upward ultrasonic sensor
- Side infrared sensor
- MPU6050 IMU

### Processing Layer

- ATmega328P microcontroller
- Analog-to-Digital Converter (ADC)
- Timer and PWM peripherals
- I²C/TWI communication
- UART communication
- Autonomous navigation state machine

### Output Layer

- Lift fan
- Thrust fan
- Steering servo
- Status LED

The microcontroller processes sensor measurements, determines the appropriate navigation state and generates the required actuator commands.

## 6. Pin Configuration

The following table presents the proposed firmware pin assignments.

| Component | Arduino Equivalent | AVR Pin | Peripheral |
|---|---|---|---|
| Lift fan | D5 | PD5 | OC0B |
| Thrust fan | D6 | PD6 | OC0A |
| Infrared sensor | A2 | PC2 | ADC2 |
| Steering servo | D9 | PB1 | OC1A |
| Bar US trigger | D7 | PD7 | GPIO |
| Bar US echo | D3 | PD3 | GPIO |
| Front US trigger | D11 | PB3 | GPIO |
| Front US echo | D2 | PD2 | GPIO |
| IMU SDA | A4 | PC4 | TWI |
| IMU SCL | A5 | PC5 | TWI |
| Status LED | D13 | PB5 | GPIO |
| UART TX | D1 | PD1 | USART |
| UART RX | D0 | PD0 | USART |

All pin assignments must be verified against the ENGR 290 controller board before hardware integration.

## 7. Microcontroller Peripheral Configuration

| Peripheral | Configuration | Purpose |
|---|---|---|
| Timer 0 | 8-bit Fast PWM | Lift and thrust fan control |
| Timer 1 | 50 Hz Fast PWM | Servo motor control |
| Timer 2 | CTC, 1 ms interrupt | System timekeeping |
| ADC | 10-bit, AVCC reference | Infrared sensor acquisition |
| TWI/I²C | 100 kHz | MPU6050 communication |
| USART | 9600 baud | Debugging and serial commands |
| GPIO | Digital input/output | Ultrasonic sensors and LED |

The peripheral allocation is designed to prevent timer conflicts between the fans, servo and system timing functions.

## 8. Software Implementation

The firmware is developed using AVR C and direct register manipulation.

### 8.1 Infrared Sensor — Exit Detection

The infrared sensor measures the distance between the hovercraft and the side wall.

The ADC converts the analog sensor voltage into a digital value between 0 and 1023.

The estimated distance is calculated using the team's empirical voltage-to-distance equation.

Exit detection requires:

- Previous confirmation of a side wall.
- Arrival at the expected exit segment.
- Multiple consecutive valid opening measurements.
- Verification of front clearance.

An invalid IR measurement alone is not considered sufficient evidence of an exit.

### 8.2 Upward Ultrasonic Sensor — Bar Detection

The upward-facing HC-SR04 detects the overhead bar.

When an overhead obstacle is confirmed:

1. The thrust fan is stopped.
2. The lift fan remains operational.
3. The steering servo returns to its neutral position.
4. The controller waits for the configured stopping interval.
5. Forward propulsion resumes.

The actual stopping distance must be determined experimentally because the hovercraft may continue coasting after thrust is removed.

### 8.3 Front Ultrasonic Sensor — Wall Detection

The forward-facing HC-SR04 measures the distance to approaching walls.

The initial navigation thresholds are:

| Front Distance | Action |
|---|---|
| Greater than 40 cm | Normal forward movement |
| 25–40 cm | Reduce thrust |
| 10–25 cm | Initiate planned turn |
| Less than 10 cm | Emergency shutdown |

These thresholds are experimental and will be adjusted during track testing.

The two ultrasonic sensors are triggered alternately to reduce acoustic interference.

### 8.4 Servo Motor — Steering

The steering servo redirects the thrust generated by the propulsion fan.

Timer 1 generates a 50 Hz PWM signal with an approximately 20 ms period.

| Servo Position | Initial Angle |
|---|---:|
| Left | 60° |
| Neutral | 90° |
| Right | 120° |

The servo's physical turning direction and mechanical limits must be calibrated.

### 8.5 MPU6050 — Heading and Turning Control

The MPU6050 communicates with the microcontroller through the I²C interface.

The gyroscope's Z-axis angular velocity is integrated over time to estimate relative yaw.

The IMU is used to:

- Maintain straight-line heading.
- Measure angular displacement.
- Execute approximately 90° turns.
- Reduce heading errors.
- Detect unsuccessful turning manoeuvres.

Gyroscope calibration is performed while the hovercraft is stationary.

### 8.6 Lift and Thrust Fans

Both fans are controlled independently using Timer 0 PWM outputs.

| Fan | Responsibility |
|---|---|
| Lift fan | Maintain the air cushion |
| Thrust fan | Control forward movement and turning |

The firmware supports independent speed adjustment and motor shutdown.

The PWM implementation assumes compatible active-high motor drivers. ESC-based propulsion systems may require a different control signal.

## 9. Autonomous Navigation

The navigation system uses a Finite State Machine (FSM) to coordinate sensor measurements and actuator commands.

| State | Description |
|---|---|
| WAIT_START | Wait for an explicit start command |
| STARTUP | Establish the air cushion |
| FORWARD | Navigate forward and maintain heading |
| BAR_STOP | Pause propulsion beneath the overhead bar |
| TURNING | Execute an IMU-controlled turn |
| EXIT_TURN | Rotate toward the detected exit |
| EXIT_FORWARD | Travel through the exit opening |
| FINISHED | Stop both fans |
| FAULT | Perform emergency shutdown |

### Navigation Sequence

1. Initialize all microcontroller peripherals.
2. Calibrate the MPU6050.
3. Wait for the start command.
4. Activate the lift fan.
5. Begin forward propulsion.
6. Continuously monitor the sensors.
7. Detect approaching walls and execute planned turns.
8. Detect the overhead bar and perform the stopping manoeuvre.
9. Identify the exit opening.
10. Turn toward the exit and move forward.
11. Stop the hovercraft at the end of the run.

The course turn sequence and exit location are configurable in the firmware.

## 10. Repository Structure

The proposed repository organization preserves the original component tests while introducing a modular AVR C implementation.

```
Hovercraft-Team1-F2026/
│
├── fans/
├── infrared/
├── servo/
├── ussensor/
│
├── AVR_Hovercraft/
│   ├── main.c
│   ├── config.h
│   ├── system.c
│   ├── system.h
│   ├── fans.c
│   ├── fans.h
│   ├── infrared.c
│   ├── infrared.h
│   ├── ultrasonic.c
│   ├── ultrasonic.h
│   ├── servo.c
│   ├── servo.h
│   ├── imu.c
│   ├── imu.h
│   └── Makefile
│
├── README.md
└── .gitignore
```

### Firmware Modules

| File | Responsibility |
|---|---|
| `main.c` | Main program and autonomous navigation |
| `config.h` | Pin assignments and calibration constants |
| `system.c` | System timer and UART communication |
| `fans.c` | Lift and thrust fan PWM control |
| `infrared.c` | ADC acquisition and IR distance estimation |
| `ultrasonic.c` | Front and overhead distance measurements |
| `servo.c` | Steering servo control |
| `imu.c` | MPU6050 communication and yaw estimation |
| `Makefile` | Firmware compilation |

Header files contain function declarations and interfaces between modules.

## 11. Development Environment

### Hardware

- ENGR 290 controller board
- ATmega328P microcontroller
- 16 MHz clock
- Compatible motor drivers
- USB serial interface or programmer

### Software

- AVR-GCC
- AVR Libc
- AVR Binutils
- AVRDUDE or a compatible programming tool
- Visual Studio Code or Microchip Studio
- Git and GitHub
- Serial terminal

### Programming Language

- AVR C
- Register-level embedded programming

## 12. Installation and Compilation

### Clone the Repository

```bash
git clone https://github.com/viviantel/Hovercraft-Team1-F2026.git
cd Hovercraft-Team1-F2026
```

### Navigate to the Firmware Directory

```bash
cd AVR_Hovercraft
```

### Compile Using Make

```bash
make
```

### Manual Compilation

```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -std=gnu11 -Wall -Wextra main.c system.c fans.c infrared.c ultrasonic.c servo.c imu.c -o hovercraft.elf -lm
```

### Generate the HEX File

```bash
avr-objcopy -O ihex -R .eeprom hovercraft.elf hovercraft.hex
```

### Check Memory Usage

```bash
avr-size -C --mcu=atmega328p hovercraft.elf
```

The generated `hovercraft.hex` file can be uploaded to the microcontroller using the appropriate programmer or bootloader.

### Clean Generated Files

```bash
make clean
```

## 13. Operating Instructions

1. Verify all electrical connections.
2. Confirm motor-driver compatibility.
3. Connect the battery and controller.
4. Keep the hovercraft stationary during IMU calibration.
5. Open the serial terminal at 9600 baud.
6. Send `S` to start the navigation sequence.
7. Send `X` to request an emergency stop.

| Command | Function |
|---|---|
| `S` | Start autonomous navigation |
| `X` | Stop the hovercraft |

The software stop command does not replace a physical emergency power cutoff.

## 14. Testing and Validation

### Individual Component Testing

- [ ] Lift fan PWM control
- [ ] Thrust fan PWM control
- [ ] Infrared sensor calibration
- [ ] Front ultrasonic distance measurement
- [ ] Overhead ultrasonic distance measurement
- [ ] Ultrasonic interference testing
- [ ] Servo angle calibration
- [ ] MPU6050 communication
- [ ] Gyroscope bias calibration
- [ ] UART communication

### Integration Testing

- [ ] Stable hovering
- [ ] Straight-line movement
- [ ] IMU-based heading correction
- [ ] Front-wall detection
- [ ] Controlled left turn
- [ ] Controlled right turn
- [ ] Overhead-bar detection
- [ ] Physical stopping-distance measurement
- [ ] Exit detection
- [ ] Exit manoeuvre
- [ ] Emergency shutdown
- [ ] Complete autonomous course navigation

### Performance Evaluation

The following parameters will be evaluated during testing:

- Course completion time
- Total distance travelled
- Number of counted components
- Heading accuracy
- Turning accuracy
- Stopping distance
- Sensor reliability
- Power consumption
- Successful overhead-bar manoeuvre
- Successful exit manoeuvre

## 15. Safety Considerations

- Never connect high-current fans directly to microcontroller GPIO pins.
- Use compatible motor drivers and suitable power supplies.
- Keep hands and loose objects away from rotating propellers.
- Perform initial firmware tests with propellers disconnected.
- Verify voltage compatibility between all electronic components.
- Calibrate the IMU while the hovercraft is stationary.
- Use a physical emergency power cutoff during testing.
- Verify sensor measurements before enabling propulsion.
- Stop the system if a critical sensor failure occurs.

## 16. Current Development Status

**Status: Prototype and integration development**

The project includes individual component implementations and a proposed integrated AVR C firmware architecture.

The integrated firmware requires compilation, hardware verification, sensor calibration and physical track testing before it can be considered competition-ready.

### Future Improvements

- Interrupt-driven ultrasonic measurements.
- Improved non-blocking sensor acquisition.
- PID-based heading and turning control.
- Battery voltage and state-of-charge monitoring.
- Improved exit confirmation.
- Optimization of fan speed and power consumption.
- Reduction of component count where practical.
- Experimental tuning of navigation thresholds.
- Improved fault detection and recovery.

## 17. Team Members

**ENGR 290 — Team 1**

- Vivian Tellili
- Candys Eve Mekoupou Eko
- Casey Tabika
- Nazila Hoche Abdi
- Rasha Mohammed
- Fabrice Pesset
- Vianelle Kayla L. K. N.

## 18. References

- ENGR 290 Course Materials — Concordia University.
- ENGR 290 Technical Assignment 1.
- ENGR 290 Technical Assignment 2.
- ATmega328P Microcontroller Datasheet.
- MPU6050 Register Map and Product Specification.
- Sharp GP2Y0A21YK0F Datasheet.
- HC-SR04 Ultrasonic Sensor Documentation.
- AVR Libc Documentation: https://www.nongnu.org/avr-libc/

## 19. Academic Project Notice

This repository is developed for educational purposes as part of the ENGR 290 Engineering Design course at Concordia University.

All hardware configurations, control algorithms and performance parameters are subject to experimental verification and improvement throughout the project.

---

**Concordia University | ENGR 290 | Team 1 | Fall 2026**

*Autonomous Navigation — Embedded Systems — Mechanical Design — Engineering Innovation*
