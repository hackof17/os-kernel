#include "uart.h"

void uart_put_hex(uint64_t x)
{
    uart_puts("0x");

    for (int i = 60; i >= 0; i -= 4) {
        uint8_t digit = (x >> i) & 0xF;
        if (digit < 10)
            uart_putc('0' + digit);
        else
            uart_putc('A' + digit - 10);
    }
}


void uart_putc(char c)
{
    // 等待 TX FIFO 非满
    while (UART_FR & (1 << 5)) {
    }

    UART_DR = (uint32_t)c;
}

void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}
