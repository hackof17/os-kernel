// vector.S
.global vectors
.extern irq_handler

.align 11          // 2KB 对齐
vectors:

/* Synchronous */
b .

/* IRQ */
b el1_irq

/* FIQ */
b .

/* SError */
b .

/* ======================
 * IRQ handler
 * ====================== */
el1_irq:
    // 保存用到的寄存器
    sub sp, sp, #16       // 16 字节足够存 x0
    str x0, [sp]

    bl irq_handler

    ldr x0, [sp]
    add sp, sp, #16

    eret
