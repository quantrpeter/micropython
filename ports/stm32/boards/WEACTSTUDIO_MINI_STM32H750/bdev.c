/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2019 Damien P. George
 */

#include "storage.h"
#include "qspi.h"

#if BUILDING_MBOOT

// Mboot uses software SPI for the first flash
#include "drivers/bus/spi.h"

static const mp_soft_spi_obj_t soft_spi_bus = {
    .delay_half = MICROPY_HW_SOFTSPI_MIN_DELAY,
    .polarity = 0,
    .phase = 0,
    .sck = MICROPY_HW_SPIFLASH_SCK,
    .mosi = MICROPY_HW_SPIFLASH_MOSI,
    .miso = MICROPY_HW_SPIFLASH_MISO,
};

const mp_spiflash_config_t spiflash_config = {
    .bus_kind = MP_SPIFLASH_BUS_SPI,
    .bus.u_spi.cs = MICROPY_HW_SPIFLASH_CS,
    .bus.u_spi.data = (void *)&soft_spi_bus,
    .bus.u_spi.proto = &mp_soft_spi_proto,
};

spi_bdev_t spi_bdev;

// Second external SPI flash uses hardware QSPI interface
const mp_spiflash_config_t spiflash2_config = {
    .bus_kind = MP_SPIFLASH_BUS_QSPI,
    .bus.u_qspi.data = NULL,
    .bus.u_qspi.proto = &qspi_proto,
};

spi_bdev_t spi_bdev2;

#else

// Normal build uses hardware SPI
#include "spi.h"

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

// Second external SPI flash uses hardware QSPI interface
const mp_spiflash_config_t spiflash2_config = {
    .bus_kind = MP_SPIFLASH_BUS_QSPI,
    .bus.u_qspi.data = NULL,
    .bus.u_qspi.proto = &qspi_proto,
    #if MICROPY_HW_SPIFLASH_ENABLE_CACHE
    .cache = &spi_bdev_cache,
    #endif
};

spi_bdev_t spi_bdev2;

#endif
 