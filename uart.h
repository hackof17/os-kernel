#pragma once
#include <stdint.h>

#define UART0_BASE 0x09000000UL

#define UART_DR (*(volatile uint32_t *)(UART0_BASE + 0x00))
#define UART_FR (*(volatile uint32_t *)(UART0_BASE + 0x18))

void uart_putc(char c);
void uart_puts(const char *s);
void uart_put_hex(uint64_t x);