#include "uart.h"
#include "gic.h"

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
    uart_puts("Timer frequency: ");
    uart_put_hex(freq);
    uart_puts("\n");

    // 装载 1 秒
    write_cntp_tval_el0(freq);

    // bit0=1 使能定时器, bit1=0 IRQ 未屏蔽
    write_cntp_ctl_el0(1);

    // 使能定时器中断（PPI 27）
    gic_enable_irq(TIMER_IRQ);
    gic_set_priority(TIMER_IRQ, 0x80);
    
    uart_puts("Timer interrupt enabled (IRQ 27)\n");
    uart_puts("timer init ok\n");
}

/* ======================
 * IRQ 处理函数 - 修改版本
 * ====================== */
void irq_handler(void)
{
    // 读取当前中断ID
    uint32_t irq_id = gic_acknowledge_irq();
    
    // 检查是否为伪中断（1023）
    if (irq_id >= 1023) {
        return;
    }
    
    uart_puts("IRQ received, ID: ");
    uart_put_hex(irq_id);
    uart_puts("\n");
    
    switch (irq_id) {
        case TIMER_IRQ: {
            // 处理定时器中断
            uint64_t ctl = read_cntp_ctl_el0();
            
            // bit[2] = ISTATUS，定时器中断触发
            if (ctl & (1 << 2)) {
                uart_puts("Timer interrupt!\n");
                
                // 重新装载下一次
                write_cntp_tval_el0(read_cntfrq_el0());
                
                // 清除定时器中断状态
                write_cntp_ctl_el0(ctl | (1 << 2));
            }
            break;
        }
        
        default:
            uart_puts("Unknown IRQ\n");
            break;
    }
    
    // 通知GIC中断处理完成
    gic_end_irq(irq_id);
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
    uart_puts("VBAR_EL1 set to vectors\n");

    uart_init();
    uart_puts("kernel start\n");
    
    // 初始化GIC
    gic_init();
    
    // 初始化定时器
    timer_init();
    
    // 启用IRQ
    enable_irq();
    uart_puts("IRQ enabled\n");

    // 主循环 - 等待中断
    uart_puts("Entering main loop, waiting for interrupts...\n");
    while (1) {
        asm volatile("wfi");
    }
}