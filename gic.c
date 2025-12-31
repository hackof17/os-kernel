// gic.c
#include "gic.h"
#include "uart.h"

// 初始化GICv2
void gic_init(void) {
    uart_puts("Initializing GIC...\n");
    
    // 1. 禁用所有中断
    GIC_REG32(GICD_ICENABLER0) = 0xFFFFFFFF;
    GIC_REG32(GICD_ICENABLER1) = 0xFFFFFFFF;
    
    // 2. 设置所有中断优先级为默认（不能是0xFF）
    for (int i = 0; i < 256; i += 4) {
        GIC_REG32(GICD_IPRIORITYR + i) = 0x80808080;  // 中等优先级
    }
    
    // 3. 设置所有SPI中断到CPU0
    // SPI中断号从32开始，到1019
    for (int i = 32; i < 1020; i += 4) {
        uint32_t offset = i / 4;
        GIC_REG32(GICD_ITARGETSR + (offset * 4)) = 0x01010101;  // CPU0
    }
    
    // 4. 配置中断触发类型（0=电平，1=边沿）
    // 默认都用边沿触发
    GIC_REG32(GICD_ICFGR0) = 0xAAAAAAAA;  // 中断0-15
    GIC_REG32(GICD_ICFGR1) = 0xAAAAAAAA;  // 中断16-31
    
    // 5. 使能GIC分发器
    GIC_REG32(GICD_CTLR) = 0x01;
    uart_puts("GIC distributor enabled\n");
    
    // 6. 初始化CPU接口
    GIC_REG32(GICC_PMR) = 0xFF;    // 优先级掩码：接收所有优先级中断
    GIC_REG32(GICC_BPR) = 0x00;    // 二进制点寄存器：优先级分组
    
    // 使能CPU接口
    GIC_REG32(GICC_CTLR) = 0x01;
    uart_puts("GIC CPU interface enabled\n");
}

// 使能特定中断
void gic_enable_irq(unsigned int irq) {
    if (irq < 32) {
        GIC_REG32(GICD_ISENABLER0) |= (1 << irq);
    } else {
        GIC_REG32(GICD_ISENABLER1) |= (1 << (irq - 32));
    }
}

// 禁用特定中断
void gic_disable_irq(unsigned int irq) {
    if (irq < 32) {
        GIC_REG32(GICD_ICENABLER0) |= (1 << irq);
    } else {
        GIC_REG32(GICD_ICENABLER1) |= (1 << (irq - 32));
    }
}

// 设置中断优先级
void gic_set_priority(unsigned int irq, unsigned int priority) {
    uint32_t reg = GICD_IPRIORITYR + (irq / 4) * 4;
    uint32_t shift = (irq % 4) * 8;
    uint32_t value = GIC_REG32(reg);
    
    value &= ~(0xFF << shift);      // 清除旧优先级
    value |= (priority << shift);   // 设置新优先级
    GIC_REG32(reg) = value;
}

// 读取当前中断ID
uint32_t gic_acknowledge_irq(void) {
    uint32_t iar = GIC_REG32(GICC_IAR);
    return iar & 0x3FF;  // 取低10位（中断ID）
}

// 通知GIC中断处理完成
void gic_end_irq(uint32_t irq) {
    GIC_REG32(GICC_EOIR) = irq;
}