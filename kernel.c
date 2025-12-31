#include "uart.h"

/* ======================
 * 定时器寄存器访问
 * ====================== */
static inline uint64_t read_cntfrq_el0(void)
{
    uint64_t val;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(val));
    return val;
}

static inline void write_cntp_tval_el0(uint64_t val)
{
    asm volatile("msr cntp_tval_el0, %0" :: "r"(val));
}

static inline void write_cntp_ctl_el0(uint64_t val)
{
    asm volatile("msr cntp_ctl_el0, %0" :: "r"(val));
}

static inline uint64_t read_cntp_ctl_el0(void)
{
    uint64_t val;
    asm volatile("mrs %0, cntp_ctl_el0" : "=r"(val));
    return val;
}

/* ======================
 * 打开 IRQ
 * ====================== */
static inline void enable_irq(void)
{
    asm volatile("msr daifclr, #2");   // 清 I 位
}

/* ======================
 * 定时器初始化
 * ====================== */
void timer_init(void)
{
    uint64_t freq = read_cntfrq_el0();

    // 装载 1 秒
    write_cntp_tval_el0(freq);

    // bit0=1 使能定时器, bit1=0 IRQ 未屏蔽
    write_cntp_ctl_el0(1);

    uart_puts("timer init ok\n");
}

/* ======================
 * IRQ 处理函数
 * ====================== */
void irq_handler(void)
{
    uint64_t ctl = read_cntp_ctl_el0();

    // bit[2] = ISTATUS，定时器中断触发
    if (ctl & (1 << 2)) {
        uart_puts("timer irq\n");

        // 重新装载下一次
        write_cntp_tval_el0(read_cntfrq_el0());
    }
}

/* ======================
 * 内核入口
 * ====================== */
extern void vectors(void);   // 异常向量表
void kernel_main(void)
{
    // 设置 VBAR_EL1 指向 vector 表
    asm volatile("msr vbar_el1, %0" :: "r"(vectors));
    asm volatile("isb");

    uart_init();
    uart_puts("kernel start\n");

    timer_init();
    enable_irq();

    while (1) {
        asm volatile("wfi");
    }
}
