/*
    GNU linker script for WeAct Studio STM32H750

    Memory layout for mboot + QSPI XIP configuration:

    FLASH_APP   .isr_vector
    FLASH_APP   .text (port code, HAL, startup)
    FLASH_APP   .data

    FLASH_EXT   .text_ext (py, extmod, lib, shared, drivers)

    FLASH_ROMFS (external QSPI, after XIP code)

    RAM         .data
    RAM         .bss
    RAM         .heap
    RAM         .stack
*/

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 128K    /* full internal flash */
    FLASH_APP (rx)  : ORIGIN = 0x08008000, LENGTH = 96K     /* after 32K mboot */
    FLASH_EXT (rx)  : ORIGIN = 0x90000000, LENGTH = 4096K   /* external QSPI flash, XIP */
    FLASH_ROMFS (rx): ORIGIN = 0x90400000, LENGTH = 4096K   /* external QSPI flash, ROMFS */
    DTCM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K    /* Used for storage cache */
    RAM (xrw)       : ORIGIN = 0x24000000, LENGTH = 512K    /* AXI SRAM */
    RAM_D2 (xrw)    : ORIGIN = 0x30000000, LENGTH = 288K
}

/* produce a link error if there is not this amount of RAM for these sections */
_minimum_stack_size = 2K;
_minimum_heap_size = 16K;

/* Define the stack. The stack is full descending so begins just above last byte
   of RAM. Note that EABI requires the stack to be 8-byte aligned for a call. */
_estack = ORIGIN(RAM) + LENGTH(RAM) - _estack_reserve;
_sstack = _estack - 16K; /* tunable */

/* RAM extents for the garbage collector */
_ram_start = ORIGIN(RAM);
_ram_end = ORIGIN(RAM) + LENGTH(RAM);
_heap_start = _ebss; /* heap starts just after statically allocated memory */
_heap_end = _sstack;

/* ROMFS location */
_micropy_hw_romfs_part0_start = ORIGIN(FLASH_ROMFS);
_micropy_hw_romfs_part0_size = LENGTH(FLASH_ROMFS);

/* Define output sections */
SECTIONS
{
    /* Keep spiflash init code in internal flash so startup can call it
       before QSPI memory-mapped mode is enabled. */
    .text_spiflash :
    {
        . = ALIGN(4);
        *drivers/memory/spiflash.o(.text* .rodata*)
        . = ALIGN(4);
    } >FLASH_APP

    /* Extended text section in external QSPI flash (XIP).
       This captures the bulk of the firmware code to free up internal flash.

       IMPORTANT: lib/stm32lib/CMSIS must NOT be listed here. It contains
       SystemInit() which is called by Reset_Handler before main() starts —
       long before our WeAct_H750_startup() maps the QSPI flash. Placing
       SystemInit in QSPI causes a hard fault at boot. */
    .text_ext :
    {
        . = ALIGN(4);
        *py/*(.text* .rodata*)
        *extmod/*(.text* .rodata*)
        *lib/libm_dbl/*(.text* .rodata*)
        *lib/littlefs/*(.text* .rodata*)
        *lib/oofatfs/*(.text* .rodata*)
        *lib/stm32lib/STM32H7xx_HAL_Driver/*(.text* .rodata*)
        *lib/tinyusb/*(.text* .rodata*)
        *shared/*(.text* .rodata*)
        *drivers/*(.text* .rodata*)
        . = ALIGN(4);
    } >FLASH_EXT

    .eth_buffers (NOLOAD) : {
        . = ABSOLUTE(0x30040000);
        *eth.o*(.bss.eth_dma)
    } >RAM_D2
}

/* The remaining code (port code, HAL, board init) goes to internal flash
   via common_bl.ld which aliases FLASH_ISR and FLASH_COMMON to FLASH_APP. */
INCLUDE common_bl.ld

