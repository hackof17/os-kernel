#include "uart.h"
/*计时器设置-轮询检查版*/
//获取当前寄存器的频率
static inline uint64_t read_cntfrq(void)
{
    uint64_t val;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(val));
    return val;
}
//倒计时设置
static inline void timer_set(uint64_t ticks)
{
    asm volatile("msr cntp_tval_el0, %0" :: "r"(ticks));
}
// 计时器使能
static inline void timer_enable(void)
{
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(1));
}
//判断寄存器是否到期
static inline int timer_expired(void)
{
    uint64_t val;
    asm volatile("mrs %0, cntp_ctl_el0" : "=r"(val));
    return (val & (1 << 2));   // ISTATUS
}

/*查看当前kernel EL等级*/
static inline uint64_t read_current_el(void)
{
    uint64_t el;
    asm volatile(
        "mrs %0, CurrentEL"
        : "=r"(el)
    );
    return el;
}

void kernel_main(void)
{
    uint64_t freq = read_cntfrq();

    uart_puts("Timer start\n");

    timer_set(freq*2);   // 2 second
    timer_enable();

    while (1) {
        if (timer_expired()) {
            uart_puts("tick\n");
            timer_set(freq*2);  // reset 2 second
        }
    }
}
