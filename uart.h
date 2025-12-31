#ifndef UART_H
#define UART_H
#include <stdint.h>

#define UART0_BASE 0x09000000UL

#define UART_DR   (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART_FR   (*(volatile uint32_t *)(UART0_BASE + 0x18))
#define UART_CR   (*(volatile uint32_t *)(UART0_BASE + 0x30))
#define UART_IBRD (*(volatile uint32_t *)(UART0_BASE + 0x24))
#define UART_FBRD (*(volatile uint32_t *)(UART0_BASE + 0x28))
#define UART_LCRH (*(volatile uint32_t *)(UART0_BASE + 0x2C))

void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_hex(uint64_t x);
void uart_init(void);

#endif 