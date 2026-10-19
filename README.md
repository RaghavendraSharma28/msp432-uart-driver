# MSP432 UART Command Console

**Embedded C · MSP432P401R · TI DriverLib · UART · GPIO**

A modular embedded firmware project that implements a serial command-line interface for the Texas Instruments MSP432P401R LaunchPad. The application is designed to accept commands over UART, control the onboard RGB LED, and report runtime status through a serial terminal.

The project emphasizes peripheral configuration, reusable drivers, defensive input handling, and separation between hardware access and application logic.

---

## Overview

The firmware uses the MSP432's eUSCI_A0 peripheral for UART communication and GPIO pins to control the RGB LED. A polling-based command interface processes user input and responds with status messages.

The application is designed around a small set of reusable UART functions, keeping serial communication separate from command processing.

### Key Features

- **UART driver:** Peripheral initialization, baud-rate configuration, character reception, and transmission.
- **Interactive command interface:** Processes textual commands received through a serial terminal.
- **GPIO control:** Controls the onboard RGB LED using individual color channels.
- **Input validation:** Bounded command buffer, oversized-input rejection, and unknown-command handling.
- **Terminal interaction:** Case-insensitive commands, backspace support, and CR/LF line-ending handling.
- **Runtime diagnostics:** Tracks processed commands and invalid-command counts.
- **Hardware abstraction:** Separates UART peripheral access from application-level command behavior.

## Hardware and Software

| Component | Configuration |
|---|---|
| Microcontroller | Texas Instruments MSP432P401R |
| Development board | MSP432P401R LaunchPad |
| Language | C |
| Peripheral library | TI DriverLib |
| Serial peripheral | eUSCI_A0 |
| UART pins | P1.2 / P1.3 |
| System clock | Configured for 48 MHz by the timing initialization |
| Intended serial configuration | 38,400 baud, 8 data bits, no parity, one stop bit |
| Development environment | Code Composer Studio |

The UART baud-rate configuration depends on the actual SMCLK frequency and must be verified against the board configuration.

## Command Reference

The application implements the following commands:

| Command | Description |
|---|---|
| `HELP` | Lists supported commands |
| `INFO` | Displays target and firmware information |
| `STATUS` | Displays command counters and the reported LED state |
| `LED RED` | Selects the red LED channel |
| `LED GREEN` | Selects the green LED channel |
| `LED BLUE` | Selects the blue LED channel |
| `LED WHITE` | Selects all three RGB channels |
| `LED OFF` | Clears the RGB LED outputs |
| `CLEAR` | Sends terminal-clear escape sequences |

Commands are converted to uppercase before processing. Invalid commands produce an error response, and oversized input lines are rejected.

**Hardware validation is pending.** The command list describes the implemented firmware behavior; physical LED operation and serial communication still need to be verified on the LaunchPad.

## Architecture

The project is organized into three main layers:

### UART Driver

`HAL/UART.c` and `HAL/UART.h` encapsulate UART initialization, baud-rate selection, character reception, and character transmission.

### Application

`src/main.c` initializes the system clock and UART, receives terminal input, parses commands, and invokes the relevant GPIO or reporting functions.

### Platform Support

`HAL/Timer.c`, `HAL/Timer.h`, `system_msp432p401r.c`, and `msp432p401r.cmd` provide system timing, device startup support, and memory-layout configuration.

## Repository Structure

```text
msp432-uart-driver/
├── HAL/
│   ├── UART.c
│   ├── UART.h
│   ├── Timer.c
│   └── Timer.h
├── src/
│   └── main.c
├── targetConfigs/
│   └── MSP432P401R.ccxml
├── msp432p401r.cmd
├── system_msp432p401r.c
├── .gitignore
└── README.md
```

The target configuration filename and project metadata may vary depending on the CCS workspace. This tree represents the intended source organization; generated build artifacts are excluded.

## Build Environment

The project is intended for Code Composer Studio with the MSP432P401R device support package and TI DriverLib.

To prepare a build:

1. Import or create a CCS project targeting the MSP432P401R.
2. Add the application, UART driver, timer implementation, startup source, and linker command file.
3. Configure the TI compiler and DriverLib include and library paths.
4. Ensure the project uses the expected 48 MHz system-clock initialization.
5. Build the project and resolve any configuration-specific errors.

The exact CCS compiler settings and DriverLib installation path depend on the local development environment.

## Verification Plan

The following checks remain to be completed on physical hardware:

- Confirm the firmware builds successfully in CCS.
- Flash the application to the MSP432P401R.
- Verify the serial prompt and communication at the configured baud rate.
- Exercise every supported command.
- Verify the RGB LED's physical behavior for each color command.
- Test lowercase input, backspace, CR/LF handling, invalid commands, and oversized input.
- Confirm that the firmware recovers correctly after an invalid or oversized command.

Hardware results and demonstration evidence will be added after testing is possible.

## Engineering Focus

This project explores embedded C design, UART peripheral configuration, GPIO control, command parsing, bounded input handling, and the organization of reusable hardware drivers.

The next development stage is to validate the firmware on the LaunchPad, confirm the baud-rate settings, and document the observed behavior.

## Author

**Raghavendra Sharma**  
Computer Engineering · Virginia Tech

Interests: Embedded Systems · Firmware Development · FPGA Design · Digital Hardware

---

*Status: Firmware implementation prepared; hardware validation pending.*