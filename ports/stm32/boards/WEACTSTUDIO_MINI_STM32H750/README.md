# 1. Build mpy-cross (host tool)
cd mpy-cross && make

# 2. Build mboot (bootloader)
cd ports/stm32/mboot
make BOARD=WEACTSTUDIO_MINI_STM32H750

# 3. Flash mboot via DFU (hold BOOT0 during reset)
dfu-util --alt 0 -D build-WEACTSTUDIO_MINI_STM32H750/firmware.dfu

# 4. Build main firmware
cd ports/stm32
make BOARD=WEACTSTUDIO_MINI_STM32H750

# 5. Flash firmware via mboot DFU (hold K1 during reset)
dfu-util --alt 0 -D build-WEACTSTUDIO_MINI_STM32H750/firmware.dfu

```
export PATH="/usr/local/opt/arm-gcc-bin@14/bin:$PATH" && \
cd /Users/peter/workspace/micropython/ports/stm32/mboot && make BOARD=WEACTSTUDIO_MINI_STM32H750 -j4 && \
cd /Users/peter/workspace/micropython/ports/stm32 && make BOARD=WEACTSTUDIO_MINI_STM32H750 -j4 && \
python3 - <<'PY'
from pathlib import Path
base = Path('/Users/peter/workspace/micropython/ports/stm32')
mboot = (base / 'mboot/build-WEACTSTUDIO_MINI_STM32H750/firmware.bin').read_bytes()
fw0 = (base / 'build-WEACTSTUDIO_MINI_STM32H750/firmware0.bin').read_bytes()
out = bytearray([0xFF] * (128 * 1024))
out[:len(mboot)] = mboot
out[0x8000:0x8000 + len(fw0)] = fw0
(base / 'build-WEACTSTUDIO_MINI_STM32H750/internal_combined.bin').write_bytes(out)
print("wrote", base / 'build-WEACTSTUDIO_MINI_STM32H750/internal_combined.bin')
PY
```