USE_MBOOT ?= 0

# MCU settings
MCU_SERIES = h7
CMSIS_MCU = STM32H750xx
MICROPY_FLOAT_IMPL = double
AF_FILE = boards/stm32h743_af.csv

# When using Mboot the firmware is split between internal flash and external QSPI.
# Internal flash (96KB after mboot) holds ISR vector, port code, HAL.
# External QSPI flash (XIP) holds the bulk of the firmware (py, extmod, lib, etc).
LD_FILES = boards/WEACTSTUDIO_MINI_STM32H750/weact_stm32h750.ld
TEXT0_ADDR = 0x08008000
TEXT1_ADDR = 0x90000000
TEXT0_SECTIONS = .isr_vector .text .data .ARM
TEXT1_SECTIONS = .text_ext


# MicroPython settings
MICROPY_VFS_LFS2 = 1

FROZEN_MANIFEST ?= $(BOARD_DIR)/manifest.py

