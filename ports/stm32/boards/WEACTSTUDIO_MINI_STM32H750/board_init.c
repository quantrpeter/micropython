/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2019 Damien P. George
 */

#include "py/mphal.h"
#include "powerctrl.h"
#include "qspi.h"
#include "storage.h"

#if !BUILDING_MBOOT

// Called very early during startup (MICROPY_BOARD_STARTUP), before clock
// configuration and before caches are enabled. This must initialize the
// QSPI flash in memory-mapped mode so that code in external flash (XIP)
// can be executed.
void WeAct_H750_startup(void) {
    // Check if we should enter the mboot bootloader.
    powerctrl_check_enter_bootloader();

    // Standard spiflash init does robust chip setup and leaves QSPI in
    // memory-mapped mode via qspi_proto.
    spi_bdev_ioctl(&spi_bdev2, BDEV_IOCTL_INIT, (uint32_t)&spiflash2_config);
}

// Called after clocks are configured (MICROPY_BOARD_EARLY_INIT).
void WeAct_H750_early_init(void) {
    // Turn off the USB switch (same behaviour as H743 board definition).
    mp_hal_pin_output(pyb_pin_OTG_FS_POWER);
    mp_hal_pin_low(pyb_pin_OTG_FS_POWER);
}

#endif // !BUILDING_MBOOT

// Called during mboot startup.
void WeAct_H750_mboot_early_init(void) {
    // Nothing special needed for mboot on this board.
}

