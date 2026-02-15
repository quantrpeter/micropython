### WeAct Studio STM32H750

The WeAct Studio Mini STM32H750 board has only 128KB of internal flash.
MicroPython uses mboot as a bootloader and runs most of the firmware from
external QSPI flash (XIP - Execute In Place).

### Building

Build mboot first, then the main firmware:

```bash
# Build and deploy mboot (from stm32/mboot/ directory)
make BOARD=WEACTSTUDIO_MINI_STM32H750 clean all

# Build the main firmware (from stm32/ directory)
make BOARD=WEACTSTUDIO_MINI_STM32H750 clean all
```

### Flashing mboot (first time)

Enter DFU mode by holding BOOT0 while pressing reset, then:

```bash
cd mboot
make BOARD=WEACTSTUDIO_MINI_STM32H750 deploy
```

Or with dfu-util:

```bash
dfu-util --alt 0 -D build-WEACTSTUDIO_MINI_STM32H750/firmware.dfu
```

### Flashing MicroPython firmware

Once mboot is installed, enter mboot DFU mode by holding the K1 button
while pressing reset. The blue LED will blink to indicate mboot is active.

Then deploy the firmware:

```bash
make BOARD=WEACTSTUDIO_MINI_STM32H750 deploy
```

Or with dfu-util:

```bash
dfu-util --alt 0 -D build-WEACTSTUDIO_MINI_STM32H750/firmware.dfu
```

The DFU file contains two segments: one for internal flash and one for
external QSPI flash. Mboot will program both automatically.

### Alternative: Using STM32CubeProgrammer

You can also use [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
to flash mboot via the built-in USB DFU bootloader (hold BOOT0 during reset).

