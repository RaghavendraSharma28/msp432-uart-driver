
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <HAL/Timer.h>
#include <HAL/UART.h>

#define COMMAND_BUFFER_SIZE 64

#define RGB_PORT GPIO_PORT_P2
#define RGB_RED GPIO_PIN0
#define RGB_GREEN GPIO_PIN1
#define RGB_BLUE GPIO_PIN2
#define RGB_ALL (RGB_RED | RGB_GREEN | RGB_BLUE)

#define FIRMWARE_VERSION "1.0.0"

static UART uart;
static char commandBuffer[COMMAND_BUFFER_SIZE];
static uint32_t commandCount = 0;
static uint32_t errorCount = 0;
static uint8_t ledState = 0;
static uint32_t bufferLength = 0;
static bool lineOverflow = false;
static bool previousWasCR = false;

static void sendLine(const char *text)
{
    UART_sendString(&uart, text);
    UART_sendString(&uart, "\r\n");
}

static void initializeLED(void)
{
    GPIO_setAsOutputPin(RGB_PORT, RGB_ALL);
    GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL);
    ledState = 0;
}

static void setLED(uint8_t state)
{
    GPIO_setOutputLowOnPin(RGB_PORT, RGB_ALL);

    if (state != 0) {
        GPIO_setOutputHighOnPin(RGB_PORT, state & RGB_ALL);
    }

    ledState = state & RGB_ALL;
}

static const char *ledName(void)
{
    switch (ledState) {
    case RGB_RED:
        return "RED";
    case RGB_GREEN:
        return "GREEN";
    case RGB_BLUE:
        return "BLUE";
    case RGB_ALL:
        return "WHITE";
    case 0:
        return "OFF";
    default:
        return "CUSTOM";
    }
}

static void showHelp(void)
{
    sendLine("");
    sendLine("Available commands:");
    sendLine("  HELP       - Show available commands");
    sendLine("  INFO       - Show firmware information");
    sendLine("  STATUS     - Show runtime status");
    sendLine("  LED RED    - Set LED to red");
    sendLine("  LED GREEN  - Set LED to green");
    sendLine("  LED BLUE   - Set LED to blue");
    sendLine("  LED WHITE  - Turn on all RGB channels");
    sendLine("  LED OFF    - Turn off RGB LED");
    sendLine("  CLEAR      - Clear terminal");
    sendLine("");
}

static void showStatus(void)
{
    char message[96];

    sendLine("");
    sendLine("Runtime status:");

    snprintf(message, sizeof(message),
             "Commands processed: %lu",
             (unsigned long)commandCount);
    sendLine(message);

    snprintf(message, sizeof(message),
             "Invalid commands: %lu",
             (unsigned long)errorCount);
    sendLine(message);

    snprintf(message, sizeof(message),
             "RGB LED: %s", ledName());
    sendLine(message);

    sendLine("");
}

static void processCommand(char *command)
{
    char message[96];

    while (*command == ' ') {
        command++;
    }

    size_t length = strlen(command);

    while (length > 0 && command[length - 1] == ' ') {
        command[--length] = '\0';
    }

    for (size_t i = 0; command[i] != '\0'; i++) {
        command[i] = (char)toupper((unsigned char)command[i]);
    }

    if (command[0] == '\0') {
        return;
    }

    commandCount++;

    if (strcmp(command, "HELP") == 0) {
        showHelp();
    } else if (strcmp(command, "INFO") == 0) {
        sendLine("");
        sendLine("MSP432 UART Command Console");
        sendLine("Target: MSP432P401R");
        sendLine("UART: eUSCI_A0, 38400 baud, 8-N-1");
        sendLine("Firmware: " FIRMWARE_VERSION);
        sendLine("");
    } else if (strcmp(command, "STATUS") == 0) {
        showStatus();
    } else if (strcmp(command, "CLEAR") == 0) {
        UART_sendString(&uart, "\033[2J\033[H");
    } else if (strcmp(command, "LED RED") == 0) {
        setLED(RGB_RED);
        sendLine("OK: RGB LED set to RED");
    } else if (strcmp(command, "LED GREEN") == 0) {
        setLED(RGB_GREEN);
        sendLine("OK: RGB LED set to GREEN");
    } else if (strcmp(command, "LED BLUE") == 0) {
        setLED(RGB_BLUE);
        sendLine("OK: RGB LED set to BLUE");
    } else if (strcmp(command, "LED WHITE") == 0) {
        setLED(RGB_ALL);
        sendLine("OK: RGB LED set to WHITE");
    } else if (strcmp(command, "LED OFF") == 0) {
        setLED(0);
        sendLine("OK: RGB LED turned OFF");
    } else {
        errorCount++;
        snprintf(message, sizeof(message),
                 "ERR: Unknown command: %s", command);
        sendLine(message);
        sendLine("Type HELP to list commands.");
    }
}

static void handleReceivedChar(char c)
{
    if (c == '\0') {
        return;
    }

    if (c == '\n' && previousWasCR) {
        previousWasCR = false;
        return;
    }

    previousWasCR = (c == '\r');

    if (c == '\r' || c == '\n') {
        UART_sendString(&uart, "\r\n");

        if (lineOverflow) {
            sendLine("ERR: Command too long; input discarded.");
            errorCount++;
        } else {
            commandBuffer[bufferLength] = '\0';
            processCommand(commandBuffer);
        }

        bufferLength = 0;
        lineOverflow = false;
        UART_sendString(&uart, "msp432> ");
        return;
    }

    if (c == '\b' || c == 127) {
        if (bufferLength > 0 && !lineOverflow) {
            bufferLength--;
            UART_sendString(&uart, "\b \b");
        }
        return;
    }

    if (c < 32 || c > 126) {
        return;
    }

    if (lineOverflow) {
        return;
    }

    if (bufferLength < COMMAND_BUFFER_SIZE - 1) {
        commandBuffer[bufferLength++] = c;
        UART_sendChar(&uart, c);
    } else {
        lineOverflow = true;
    }
}

int main(void)
{
    WDT_A_holdTimer();
    InitSystemTiming();

    initializeLED();

    uart = UART_construct(
        USB_UART_INSTANCE,
        USB_UART_PORT,
        USB_UART_PINS
    );

    if (!UART_SetBaud_Enable(&uart, BAUD_38400)) {
        while (1) {
        }
    }

    sendLine("");
    sendLine("==============================");
    sendLine(" MSP432 UART COMMAND CONSOLE ");
    sendLine("==============================");
    sendLine("Type HELP to get started.");
    sendLine("");

    UART_sendString(&uart, "msp432> ");

    while (1) {
        if (UART_hasChar(&uart)) {
            handleReceivedChar(UART_getChar(&uart));
        }
    }
}
