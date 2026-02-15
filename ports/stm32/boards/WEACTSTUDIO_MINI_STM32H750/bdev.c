/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2019 Damien P. George
 */

#include "storage.h"
#include "qspi.h"

#if !BUILDING_MBOOT

// The first external SPI flash (W25Q64 on SPI1) is only used by the main
// firmware for R/W storage, not by mboot.
#include "spi.h"
#include "py/mpconfig.h"

static const spi_proto_cfg_t spi_bus = {
    .spi = &spi_obj[0], // SPI1
    .baudrate = 25000000,
    .polarity = 0,
    .phase = 0,
    .bits = 8,
    .firstbit = SPI_FIRSTBIT_MSB,
};

#if MICROPY_HW_SPIFLASH_ENABLE_CACHE
static mp_spiflash_cache_t spi_bdev_cache;
#endif

const mp_spiflash_config_t spiflash_config = {
    .bus_kind = MP_SPIFLASH_BUS_SPI,
    .bus.u_spi.cs = MICROPY_HW_SPIFLASH_CS,

    .bus.u_spi.data = (void *)&spi_bus,
    .bus.u_spi.proto = &spi_proto,
    #if MICROPY_HW_SPIFLASH_ENABLE_CACHE
    .cache = &spi_bdev_cache,
    #endif
};

spi_bdev_t spi_bdev;

#endif // !BUILDING_MBOOT

// Second external SPI flash uses hardware QSPI interface.
// This is used by both mboot (for programming) and the main firmware (for XIP and ROMFS).
const mp_spiflash_config_t spiflash2_config = {
    .bus_kind = MP_SPIFLASH_BUS_QSPI,
    .bus.u_qspi.data = NULL,
    .bus.u_qspi.proto = &qspi_proto,
    #if !BUILDING_MBOOT && MICROPY_HW_SPIFLASH_ENABLE_CACHE
    .cache = &spi_bdev_cache,
    #endif
};

spi_bdev_t spi_bdev2;
