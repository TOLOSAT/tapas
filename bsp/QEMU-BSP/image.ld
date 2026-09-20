/* Copyright (c) TOLOSAT 2026 */
/* SPDX-License-Identifier: Apache-2.0 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
    ISR     (rwx) : ORIGIN = 0x00000000, LENGTH = 64k  /* Vector table in QEMU internal ROM */
    SRAM1   (rwx) : ORIGIN = 0x20000000, LENGTH = 512k /* Protected zone */
    SRAM2   (rwx) : ORIGIN = 0x20080000, LENGTH = 512k
    PSRAM   (rw)  : ORIGIN = 0x60000000, LENGTH = 16M
}

/*
 * Note : These zones have been artificially created in the ZBT
 *        SSRAMs (with the exception of the PSRAM) of the cmsdk
 *        in order to emulate the behaviour of STM32H7s.
 */

/* Stack & Heap definitions */
__stack_end__       = ORIGIN(SRAM1) + LENGTH(SRAM1);
__Min_Heap_Size__   = 0x200;
__Min_Stack_Size__  = 0x400;

/* Sections */
SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >ISR

    /* The protected program (kernel) goes into the .text section */
    .protected_text :
    {
        . = ALIGN(4);
        __kernel_text_start__ = .;
        *(.text.Default_Handler)
        *(.text.Reset_Handler)
        *(.text.SystemInit)
        *(.text.SystemClock_Config)
        *libkernel*.a:*(.text .text.*)
        *libfreertos*.a:*(.text .text.*)
        *libfatfs*.a:*(.text .text.*)
        *libhal*.a:*(.text .text.*)
        . = ALIGN(4);
        __kernel_text_end__ = .;
    } >SRAM1

    /* ARM exception handling indexes goes after protected text memory */
    .ARM.exidx : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(0x20000); /* Align on 128KB for MPU RO region */
        __exidx_end = .;
    } >SRAM1

    /* Protected constant data goes into .rodata section */
    .protected_rodata :
    {
        . = ALIGN(4);
        __kernel_rodata_start__ = .;
        *libkernel*.a:*(.rodata .rodata.*)
        *libfreertos*.a:*(.rodata .rodata.*)
        *libfatfs*.a:*(.rodata .rodata.*)
        *libhal*.a:*(.rodata .rodata.*)
        . = ALIGN(4);
        __kernel_rodata_end__ = .;
    } >SRAM1

    /* ARM exception handling tables goes after protected rodata memory */
    .ARM.extab :
    {
        . = ALIGN(4);
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(0x20000); /* Align on 128KB for MPU RO XN region */
        __extab_end = .;
    } >SRAM1

    /* Protected initialized data go into the .data section */
    .protected_data :
    {
        . = ALIGN(4);
        __kernel_data_start__ = .;
        *(.data.SystemCoreClock)
        *(.data.SystemD2Clock)
        *libkernel*.a:*(.data .data.*)
        *libfreertos*.a:*(.data .data.*)
        *libfatfs*.a:*(.data .data.*)
        *libhal*.a:*(.data .data.*)
        . = ALIGN(4);
        __kernel_data_end__ = .;
    } >SRAM1

    /* Protected uninitialised data goes into .bss section */
    .protected_bss (NOLOAD):
    {
        . = ALIGN(4);
        __kernel_bss_start__ = .;
        *libkernel*.a:*(.bss .bss.*)
        *libfreertos*.a:*(.bss .bss.*)
        *libfatfs*.a:*(.bss .bss.*)
        *libhal*.a:*(.bss .bss.*)
        . = ALIGN(4);
        __kernel_bss_end__ = .;
    } >SRAM1

    /* Use of the rest of SRAM1 for the main stack */
    .stack :
    {
        . = ALIGN(8);
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >SRAM1

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        __text_start__ = .;
        __syscalls_start__ = .;
        *(.syscalls)
        . = ALIGN(4);
        __syscalls_end__ = .;
        *(.text)
        *(.text*)
        . = ALIGN(4);
        __text_end__ = .;
    } >SRAM2

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        *(.config)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        __rodata_end__ = .;
    } >SRAM2

    /* Initialized data go into the .data section */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >SRAM2

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        *(.task_stacks)
        . = ALIGN(4);
        __bss_end__ = .;
    } >SRAM2

    /* The dma buffers goes into .dmabuff to avoid saturating the AXI SRAM. */
    .dmabuff :
    {
        . = ALIGN(4);
        __dmabuff_start__ = .;
        *(.dmabuff)
        . = ALIGN(4);
        __dmabuff_end__ = .;
    } >SRAM2

    /* Use of the rest of SRAM for the heap (should remain empty) */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
        . = ALIGN(8);
    } >SRAM2

    /* PSRAM zone used for RAM file system */
    .ramfs (NOLOAD):
    {
        . = ALIGN(4);
        __ramfs_start__ = .;
        . = . + LENGTH(PSRAM);
        __ramfs_end__ = .;
        . = ALIGN(4);
    } >PSRAM
}
