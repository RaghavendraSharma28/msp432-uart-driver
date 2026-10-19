
#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <stdbool.h>
#include <stdint.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define UART_Config eUSCI_A_UART_ConfigV1

#define USB_UART_PORT GPIO_PORT_P1
#define USB_UART_PINS (GPIO_PIN2 | GPIO_PIN3)
#define USB_UART_INSTANCE EUSCI_A0_BASE

typedef enum {
    BAUD_38400,
    BAUD_57600,
    NUM_BAUD_CHOICES
} UART_Baudrate;

typedef struct {
    UART_Config config;
    uint32_t moduleInstance;
    uint32_t port;
    uint32_t pins;
} UART;

UART UART_construct(uint32_t moduleInstance, uint32_t port, uint32_t pins);
bool UART_SetBaud_Enable(UART *uart, UART_Baudrate baudrate);
bool UART_hasChar(UART *uart);
char UART_getChar(UART *uart);
bool UART_canSend(UART *uart);
void UART_sendChar(UART *uart, char c);
void UART_sendString(UART *uart, const char *str);

#endif
