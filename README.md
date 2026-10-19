# MSP432 UART Command Console

**Embedded C · MSP432P401R · TI DriverLib · UART · GPIO**

A modular embedded firmware project that provides a serial command-line interface for an MSP432P401R LaunchPad. Commands received over UART allow a user to inspect firmware status and control the onboard RGB LED.

## Features

- UART communication using eUSCI_A0
- Configurable baud-rate driver
- Interactive command parser
- RGB LED control through GPIO
- Runtime command and error counters
- Input validation and bounded command buffering
- Backspace support and CR/LF line-ending handling
- Modular UART driver and application code

## Hardware

- Texas Instruments MSP432P401R LaunchPad
- USB connection for the board's backchannel UART
- Code Composer Studio and the MSP432 DriverLib

## UART Configuration

| Setting | Value |
|---|---|
| Peripheral | eUSCI_A0 |
| Pins | P1.2 / P1.3 |
| Baud rate | 38,400 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Transfer model | Polling |

The UART configuration assumes SMCLK is running at 48 MHz. Verify the baud-rate settings against the actual board configuration before relying on them.

## Commands

| Command | Description |
|---|---|
| `HELP` | List supported commands |
| `INFO` | Show target and firmware information |
| `STATUS` | Show command counters and LED state |
| `LED RED` | Set the RGB LED to red |
| `LED GREEN` | Set the RGB LED to green |
| `LED BLUE` | Set the RGB LED to blue |
| `LED WHITE` | Turn on all RGB channels |
| `LED OFF` | Turn off the RGB LED |
| `CLEAR` | Clear a compatible terminal |

Commands are case-insensitive. Unknown commands return an error message.

## Repository Layout

```text
msp432-uart-command-console/
├── HAL/
│   ├── UART.c
│   ├── UART.h
│   ├── Timer.c
│   └── Timer.h
├── src/
│   └── main.c
├── docs/
├── targetConfigs/
├── msp432p401r.cmd
├── system_msp432p401r.c
├── .gitignore
└── README.md
```

## Build and Run

1. Import the project into Code Composer Studio.
2. Configure the project for the MSP432P401R and the appropriate TI compiler and DriverLib.
3. Build the project and resolve any target-specific configuration issues.
4. Connect the LaunchPad over USB and flash the firmware.
5. Open the board's backchannel serial port using a terminal configured for 38,400 baud, 8 data bits, no parity, and one stop bit.
6. Type `HELP` and press Enter to see the available commands.

The exact CCS project settings and target configuration may need to be supplied alongside the source files.

## Verification

Hardware validation should include:

- Confirming that the serial prompt appears after reset.
- Sending each supported command and checking the response.
- Verifying each RGB LED state physically.
- Testing lowercase input, backspace, CR/LF line endings, and unknown commands.
- Sending an overlength line and confirming it is rejected without corrupting subsequent commands.
- Checking communication at the configured baud rate.

Add actual test results and terminal screenshots after running these checks on hardware.

## Engineering Scope

This project explores peripheral configuration, polling-based serial communication, command parsing, GPIO control, input validation, and modular embedded C design.

## License

Add a license after confirming that the source code and any course-provided or third-party components may be redistributed under that license.
