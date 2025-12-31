// gic.h
#ifndef GIC_H
#define GIC_H

#include <stdint.h>

// GICv2 寄存器基地址（ARM VIRT平台）
#define GICD_BASE        0x08000000UL
#define GICC_BASE        0x08010000UL

// Distributor 寄存器偏移
#define GICD_CTLR        (GICD_BASE + 0x000)
#define GICD_TYPER       (GICD_BASE + 0x004)
#define GICD_ISENABLER0  (GICD_BASE + 0x100)  // 中断0-31使能
#define GICD_ISENABLER1  (GICD_BASE + 0x104)  // 中断32-63使能
#define GICD_ICENABLER0  (GICD_BASE + 0x180)
#define GICD_ICENABLER1  (GICD_BASE + 0x184)
#define GICD_ISPENDR0    (GICD_BASE + 0x200)
#define GICD_ISPENDR1    (GICD_BASE + 0x204)
#define GICD_ICPENDR0    (GICD_BASE + 0x280)
#define GICD_ICPENDR1    (GICD_BASE + 0x284)
#define GICD_ISACTIVER0  (GICD_BASE + 0x300)
#define GICD_ISACTIVER1  (GICD_BASE + 0x304)
#define GICD_ICACTIVER0  (GICD_BASE + 0x380)
#define GICD_ICACTIVER1  (GICD_BASE + 0x384)
#define GICD_IPRIORITYR  (GICD_BASE + 0x400)
#define GICD_ITARGETSR   (GICD_BASE + 0x800)
#define GICD_ICFGR0      (GICD_BASE + 0xC00)
#define GICD_ICFGR1      (GICD_BASE + 0xC04)

// CPU Interface 寄存器偏移
#define GICC_CTLR        (GICC_BASE + 0x000)
#define GICC_PMR         (GICC_BASE + 0x004)
#define GICC_BPR         (GICC_BASE + 0x008)
#define GICC_IAR         (GICC_BASE + 0x00C)
#define GICC_EOIR        (GICC_BASE + 0x010)

// 寄存器访问宏
#define GIC_REG32(addr)  (*((volatile uint32_t *)(addr)))
#define GIC_REG64(addr)  (*((volatile uint64_t *)(addr)))

// 中断号定义
#define TIMER_IRQ        27     // ARM通用定时器中断（PPI）
#define UART0_IRQ        33     // VIRT平台UART0中断（SPI）

// GIC配置函数声明
void gic_init(void);
void gic_enable_irq(unsigned int irq);
void gic_disable_irq(unsigned int irq);
void gic_set_priority(unsigned int irq, unsigned int priority);
uint32_t gic_acknowledge_irq(void);
void gic_end_irq(uint32_t irq);

#endif // GIC_H