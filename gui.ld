OUTPUT_FORMAT(binary);
SECTIONS
{
    . = 0x100;

    .text :
    {
        *(.text);
    }

    .data :
    {
        *(.data);
        *(.rodata);
        *(.bss);
    }
}
