/* Copyright (c) TOLOSAT 2026 */
/* SPDX-License-Identifier: Apache-2.0 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    DTCM_RAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
    AXI_SRAM1   (rwx) : ORIGIN = 0x24000000, LENGTH = 256K
    AXI_SRAM2   (rwx) : ORIGIN = 0x24040000, LENGTH = 256K
    SRAM1       (rwx) : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x30020000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x30040000, LENGTH = 32K
}

/* Stack & Heap definitions */
__stack_end__       = ORIGIN(DTCM_RAM) + LENGTH(DTCM_RAM);
__Min_Heap_Size__   = 0x200;
__Min_Stack_Size__  = 0x400;

/* Sections */
SECTIONS
{
    /* Par of the kernel code go into the .tcm_text section, which is faster. */
    .tcm_text :
    {
        . = ALIGN(8);
        __tcm_text_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(8);
        __kernel_tcm_text_start__ = .;
        *(.text.Default_Handler)
        *(.text.Reset_Handler)
        *(.text.SystemInit)
        *(.text.SystemClock_Config)
        *libkernel*.a:*(.text .text.*)
        . = ALIGN(8);
        __kernel_tcm_text_end__ = .;
        . = ALIGN(8);
        __tcm_text_end__ = .;
    } >ITCM_RAM

    /* ARM exception handling indexes goes after TCM text memory */
    .ARM.exidx : {
        . = ALIGN(8);
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        . = ALIGN(8);
        __exidx_end = .;
    } >ITCM_RAM

    /* Part of the kernel read only data go into the .tcm_rodata section, which is faster. */
    .tcm_rodata :
    {
        . = ALIGN(8);
        __tcm_rodata_start__ = .;
        __kernel_tcm_rodata_start__ = .;
        *libkernel*.a:*(.rodata .rodata.*)
        . = ALIGN(8);
        __kernel_tcm_rodata_end__ = .;
        . = ALIGN(8);
        __tcm_rodata_end__ = .;
    } >DTCM_RAM

    /* ARM exception handling tables goes after tcm rodata memory */
    .ARM.extab :
    {
        . = ALIGN(8);
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(0x4000); /* Align on 16KB for MPU RO region */
        __extab_end = .;
    } >DTCM_RAM

    /* Part of the kernel data go into the .tcm_data section, which is faster. */
    .tcm_data :
    {
        . = ALIGN(8);
        __tcm_data_start__ = .;
        __kernel_tcm_data_start__ = .;
        *(.data.SystemCoreClock)
        *(.data.SystemD2Clock)
        *libkernel*.a:*(.data .data.*)
        . = ALIGN(8);
        __kernel_tcm_data_end__ = .;
        . = ALIGN(8);
        __tcm_data_end__ = .;
    } >DTCM_RAM

    /* Part of the kernel bss go into the .tcm_bss section, which is faster. */
    .tcm_bss (NOLOAD) :
    {
        __tcm_bss_start__ = .;
        . = ALIGN(8);
        __kernel_tcm_bss_start__ = .;
        *libkernel*.a:*(.bss .bss.*)
        . = ALIGN(8);
        __kernel_tcm_bss_end__ = .;
        . = ALIGN(8);
        __tcm_bss_end__ = .;
    } >DTCM_RAM

    /* Use of the rest of DTCM RAM for the main stack */
    .stack :
    {
        . = ALIGN(8);
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >DTCM_RAM

    /* The program goes into the .text section */
    .protected_text :
    {
        . = ALIGN(8);
        __kernel_text_start__ = .;
        *libfreertos*.a:*(.text .text.*)
        *libfatfs*.a:*(.text .text.*)
        *libhal*.a:*(.text .text.*)
        . = ALIGN(0x20000); /* Align on 128KB for MPU RO XN region */
        __kernel_text_end__ = .;
    } >AXI_SRAM1

    /* Constant data goes into .rodata section */
    .protected_rodata :
    {
        . = ALIGN(8);
        __kernel_rodata_start__ = .;
        *libfreertos*.a:*(.rodata .rodata.*)
        *libfatfs*.a:*(.rodata .rodata.*)
        *libhal*.a:*(.rodata .rodata.*)
        . = ALIGN(0x4000); /* Align on 16KB for MPU RO region */
        __kernel_rodata_end__ = .;
    } >AXI_SRAM1

    /* Initialized data go into the .data section */
    .protected_data :
    {
        . = ALIGN(8);
        __kernel_data_start__ = .;
        *libfreertos*.a:*(.data .data.*)
        *libfatfs*.a:*(.data .data.*)
        *libhal*.a:*(.data .data.*)
        . = ALIGN(8);
        __kernel_data_end__ = .;
    } >AXI_SRAM1

    /* Uninitialised data goes into .bss section */
    .protected_bss (NOLOAD):
    {
        . = ALIGN(8);
        __kernel_bss_start__ = .;
        *libfreertos*.a:*(.bss .bss.*)
        *libfatfs*.a:*(.bss .bss.*)
        *libhal*.a:*(.bss .bss.*)
        . = ALIGN(8);
        __kernel_bss_end__ = .;
    } >AXI_SRAM1

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(8);
        __text_start__ = .;
        __syscalls_start__ = .;
        *(.syscalls)
        . = ALIGN(8);
        __syscalls_end__ = .;
        *(.text)
        *(.text*)
        . = ALIGN(8);
        __text_end__ = .;
    } >AXI_SRAM2

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(8);
        __rodata_start__ = .;
        *(.config)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(8);
        __rodata_end__ = .;
    } >AXI_SRAM2

    /* Initialized data go into the .data section */
    .data :
    {
        . = ALIGN(8);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(8);
        __data_end__ = .;
    } >AXI_SRAM2

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD):
    {
        . = ALIGN(8);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        *(.task_stacks)
        . = ALIGN(8);
        __bss_end__ = .;
    } >AXI_SRAM2

    /* Use of the rest of AXI_SRAM for the heap (should remain empty) */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
        . = ALIGN(8);
    } >AXI_SRAM2

    /* The dma buffers goes into .dmabuff to avoid saturating the AXI SRAM. */
    .dmabuff :
    {
        . = ALIGN(4);
        __dmabuff_start__ = .;
        *(.dmabuff)
        . = ALIGN(4);
        __dmabuff_end__ = .;
    } >SRAM3
}
