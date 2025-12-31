.global _start
.section .text

_start:
    // 设置栈
    ldr x0, =stack_top
    mov sp, x0

    // 清 BSS
    ldr x1, =__bss_start
    ldr x2, =__bss_end
bss_clear:
    cmp x1, x2
    b.ge bss_done
    str xzr, [x1], #8
    b bss_clear
bss_done:

    // 跳转到 C
    bl kernel_main

1:  wfe
    b 1b

.section .bss
.align 12
stack:
    .skip 4096
stack_top:

