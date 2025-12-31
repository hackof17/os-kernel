ENTRY(_start)

SECTIONS {
    /* QEMU virt 推荐加载地址 */
    . = 0x40000000;

    /* 启动代码，放最前 */
    .text.boot : {
        *(.text.boot)
    }

    /* 异常向量表：必须对齐 + 不被丢弃 */
    .vectors ALIGN(2048) : {
        KEEP(*(.vectors))
    }

    /* 普通内核代码 */
    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }
}