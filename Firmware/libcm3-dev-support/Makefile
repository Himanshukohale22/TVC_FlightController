# Toolchain
PREFIX = arm-none-eabi
CC = $(PREFIX)-gcc
LD = $(PREFIX)-gcc
OBJCOPY = $(PREFIX)-objcopy
SIZE = $(PREFIX)-size

# Project
PROJECT = stm32g0-project
SRC_DIR = src
CFILES = $(wildcard $(SRC_DIR)/*.c)
OFILES = $(CFILES:.c=.o)

# libopencm3 paths (ABSOLUTE PATH)
OPENCM3_DIR = /home/if-himanshu/Stm32G0-flow/libopencm3

# Device configuration
DEVICE = stm32g071xb
FAMILY = stm32g0

# Compiler flags
INCLUDES = -I$(OPENCM3_DIR)/include
CFLAGS = -Os -g \
         -Wall -Wextra \
         -fno-common -ffunction-sections -fdata-sections \
         -mcpu=cortex-m0plus -mthumb \
         $(INCLUDES) -D$(DEVICE) -MD

# Linker configuration - CRITICAL FIXES:
LIBOPENCM3 = $(OPENCM3_DIR)/lib/libopencm3_$(FAMILY).a
LDFLAGS = -nostartfiles -Wl,--gc-sections \
          -L$(OPENCM3_DIR)/lib \
          -T$(OPENCM3_DIR)/lib/stm32/$(FAMILY)/$(FAMILY).ld \
          -specs=nano.specs \
          $(LIBOPENCM3) -lc -lgcc -lnosys \
		  ... -T/home/if-himanshu/Stm32G0-flow/minmal.ld ...

# Build targets
all: $(PROJECT).bin

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

$(PROJECT).elf: $(OFILES) $(LIBOPENCM3)
	$(LD) -o $@ $(OFILES) $(LDFLAGS) -mcpu=cortex-m0plus -mthumb

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(OFILES:.o=.d)
	rm -f $(PROJECT).elf $(PROJECT).bin

flash: $(PROJECT).bin
	st-flash --reset write $(PROJECT).bin 0x8000000

.PHONY: all clean flash