CC = arm-none-eabi-
LD_SCRIPT = stm32l47.ld
LD_FLAGS  = -T $(LD_SCRIPT)
LD_FLAGS += -nostdlib
CC_FLAGS  = -mcpu=cortex-m4
CC_FLAGS += -mthumb
CC_FLAGS += -mfloat-abi=soft

objects = main.o

all: $(objects)
  $(CC)gcc $(CC_FLAGS) $(LD_FLAGS) main.o -o firmware.elf
  $(CC)gcc -O binary firmware.elf firmware.bin

main.o:
  $(CC)gcc $(CC_FLAGS) -c main.c -o main.o

clean:
  rm -f *.o firmware.elf
