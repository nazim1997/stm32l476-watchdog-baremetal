# STM32L476 Bare-Metal Watchdog

A bare-metal project for the **NUCLEO-L476RG** (STM32L476RG, Cortex-M4), written in C with **no HAL and no CMSIS**. Everything is built from scratch: linker script, startup code, vector table, and direct register access.

## What it does

- Blinks the user LED **LD2 (PA5)** with a **SysTick-based** millisecond delay
- Runs the **Independent Watchdog (IWDG)**, which resets the MCU if the main loop stops feeding it

## What I built from scratch

| Piece | Details |
|---|---|
| Linker script | Flash at `0x08000000` (1 MB), SRAM at `0x20000000` (96 KB), `.isr_vector`, `.data` (loaded from Flash), `.bss` |
| Startup code | `Reset_Handler` copies `.data` from Flash to SRAM, zeroes `.bss`, then calls `main()` |
| Vector table | Initial stack pointer, reset handler, and system exception stubs |
| SysTick delay | 4 MHz MSI clock, 1 ms = `0xFA0` cycles, polling `COUNTFLAG` |
| IWDG | Started with key `0xCCCC`, fed with key `0xAAAA`, default timeout of about 0.5 s |

## Project structure

```
.
├── main.c                 # GPIO, SysTick delay, IWDG, blink loop
├── startup_stm32l476.c    # Vector table and Reset_Handler
├── stm32l47.ld            # Linker script
└── Makefile
```

## Build and flash

Requirements: `arm-none-eabi-gcc`, `make`, and `st-flash` (from stlink-tools).

```bash
make           # builds firmware.elf and firmware.bin
make flash     # st-flash --reset write firmware.bin 0x08000000
make clean
```

On WSL, the ST-Link has to be passed through from Windows with [usbipd-win](https://github.com/dorssel/usbipd-win) before `st-flash` can see it.

## Testing the watchdog

- **Feeding in time:** with a blink delay well under the timeout and `iwdg_feed()` called in the loop, the LED blinks normally.
- **Not feeding in time:** with a delay longer than about 0.5 s and no feeding, the chip resets over and over, so the LED looks stuck on.
- **Watchdog disabled:** comment out `iwdg_init()` and the long delay works normally, which confirms the IWDG is what causes the reset.

## References

- RM0351: STM32L47x/L48x/L49x/L4Axx reference manual
- PM0214: STM32 Cortex-M4 programming manual
- UM1724: STM32 Nucleo-64 boards user manual
