// vector.S - 最小寄存器保存版本
.global vectors
.extern irq_handler

.section .vectors, "ax"
.align 11          // 2KB 对齐

vectors:
    /* 当前EL（使用SP0） */
    b .           // 同步异常
    .align 7
    b el1_irq     // IRQ异常 <- 重点
    .align 7
    b .           // FIQ异常
    .align 7
    b .           // SError异常
    
    /* 其他EL的向量表项（对齐即可） */
    .fill 12, 8, 0

/* ======================
 * IRQ 处理（最小版本）
 * ====================== */
.section .text
el1_irq:
    // 保存会被破坏的寄存器（按照AAPCS64标准）
    // x0-x7, x30 是调用者保存寄存器，必须保存
    sub sp, sp, #(8 * 9)    // 为 x0-x7 和 x30 分配空间
    stp x0, x1, [sp, #(8 * 0)]
    stp x2, x3, [sp, #(8 * 2)]
    stp x4, x5, [sp, #(8 * 4)]
    stp x6, x7, [sp, #(8 * 6)]
    str x30, [sp, #(8 * 8)]  // 保存返回地址
    
    // 调用C语言中断处理函数
    bl irq_handler
    
    // 恢复寄存器
    ldp x0, x1, [sp, #(8 * 0)]
    ldp x2, x3, [sp, #(8 * 2)]
    ldp x4, x5, [sp, #(8 * 4)]
    ldp x6, x7, [sp, #(8 * 6)]
    ldr x30, [sp, #(8 * 8)]
    add sp, sp, #(8 * 9)
    
    eret