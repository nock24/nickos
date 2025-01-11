SECTIONS
{
    .text.boot : { *(.text.boot) }
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    . = ALIGN(0x8);
    PROVIDE ( __bss_begin__ = . );
    .bss : { *(.bss*) }
    PROVIDE ( __bss_end__ = . );
    .heap : {
        . = ALIGN(0x8);
        PROVIDE (__heap_start__ = . );
        . = . + 0x1000;
        PROVIDE (__heap_end__ = .);
        . = ALIGN(0x8);
    }
}
