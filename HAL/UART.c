
#include <HAL/UART.h>

UART UART_construct(uint32_t moduleInstance, uint32_t port, uint32_t pins)
{
    UART uart = {0};

    uart.moduleInstance = moduleInstance;
    uart.port = port;
    uart.pins = pins;

    GPIO_setAsPeripheralModuleFunctionInputPin(
        uart.port,
        uart.pins,
        GPIO_PRIMARY_MODULE_FUNCTION
    );

    uart.config.parity = EUSCI_A_UART_NO_PARITY;
    uart.config.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    uart.config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    uart.config.uartMode = EUSCI_A_UART_MODE;
    uart.config.dataLength = EUSCI_A_UART_8_BIT_LEN;
    uart.config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    uart.config.overSampling =
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    return uart;
}

bool UART_SetBaud_Enable(UART *uart, UART_Baudrate baudrate)
{
    static const uint16_t prescaler[NUM_BAUD_CHOICES] = {
        78, 52
    };

    static const uint8_t firstModReg[NUM_BAUD_CHOICES] = {
        2, 1
    };

    static const uint8_t secondModReg[NUM_BAUD_CHOICES] = {
        0, 0
    };

    if (uart == 0 || baudrate >= NUM_BAUD_CHOICES) {
        return false;
    }

    uart->config.clockPrescalar = prescaler[baudrate];
    uart->config.firstModReg = firstModReg[baudrate];
    uart->config.secondModReg = secondModReg[baudrate];

    UART_initModule(uart->moduleInstance, &uart->config);
    UART_enableModule(uart->moduleInstance);

    return true;
}

bool UART_hasChar(UART *uart)
{
    return UART_getInterruptStatus(
        uart->moduleInstance,
        EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG
    ) != 0;
}

char UART_getChar(UART *uart)
{
    if (!UART_hasChar(uart)) {
        return '\0';
    }

    return (char)UART_receiveData(uart->moduleInstance);
}

bool UART_canSend(UART *uart)
{
    return UART_getInterruptStatus(
        uart->moduleInstance,
        EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG
    ) != 0;
}

void UART_sendChar(UART *uart, char c)
{
    while (!UART_canSend(uart)) {
    }

    UART_transmitData(uart->moduleInstance, (uint8_t)c);
}

void UART_sendString(UART *uart, const char *str)
{
    if (uart == 0 || str == 0) {
        return;
    }

    while (*str != '\0') {
        UART_sendChar(uart, *str++);
    }
}
