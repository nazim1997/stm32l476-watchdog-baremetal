CC = arm-none-eabi-gcc
LD_SCRIPT = stm32l47.ld
LD_FLAGS  = -T $(LD_SCRIPT)
LD_FLAGS += -nostdlib
CC_FLAGS  = -mcpu=cortex-m4
CC_FLAGS += -mthumb
CC_FLAGS += -mfloat-abi=soft

objects = main.o startup.o

all: $(objects)
        $(CC) $(CC_FLAGS) $(LD_FLAGS) $(objects) -o firmware.elf
        arm-none-eabi-objcopy -O binary firmware.elf firmware.bin

main.o:
        $(CC) $(CC_FLAGS) -c main.c -o main.o

startup.o: startup.c
        $(CC) $(CC_FLAGS) -c startup.c -o startup.o

flash:
        st-flash --reset write firmware.bin 0x08000000

clean:
        rm -f *.o firmware.elf firmware.bin
