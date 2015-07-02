
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar
AS = $(PREFIX)as
LINK = $(PREFIX)gcc
SIZE = $(PREFIX)size
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
GDBTUI = $(PREFIX)gdb --tui
FLASHER = st-flash
DEBUGSERVER = st-util

# define build specific options
CPU_USAGE = -mcpu=cortex-m4
FPU_USAGE = -mfloat-abi=hard -mfpu=fpv4-sp-d16

LINKERSCRIPT = stm32f407vg_linkerscript_flash.ld

TGT = stm32f4discovery
BIN = $(TGT).bin
LIB = lib$(TGT).a

CFLAGS += -ggdb -g3 -std=gnu99 -Os -Wall -Wstrict-prototypes $(CPU_USAGE) $(FPU_USAGE) -mlittle-endian -mthumb -mthumb-interwork -nostartfiles
CFLAGS += -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += -I. -I./app/inc/ -I./board/ -I./lib/inc -I./lib/inc/core -I./lib/inc/peripherals
CFLAGS += -I../../Source/include -I../../Source/portable/GCC/ARM_CM4F -I../Common/include
ASFLAGS += -ggdb -g3 $(CPU_USAGE) $(FPU_USAGE) -mlittle-endian
LINKFLAGS += -g3 -ggdb -std=gnu99 $(CPU_USAGE) $(FPU_USAGE) -mlittle-endian -static -lgcc -mthumb -mthumb-interwork -nostartfiles
LINKFLAGS += -T$(LINKERSCRIPT)
OFLAGS = -O binary
FFLAGS = write $(BIN) 0x08000000

# unwanted (CXXUWFLAGS) and extra (CXXEXFLAGS) flags for c++
CXXUWFLAGS +=
CXXEXFLAGS +=

# use newLib nano-specs if available
ifeq ($(shell $(LINK) -specs=nano.specs -E - 2>/dev/null >/dev/null </dev/null ; echo $$?),0)
LINKFLAGS += -specs=nano.specs -lc -lnosys
endif

CSRC = main.c \
	   $(addprefix startup/, system_stm32f4xx.c) \
	   $(addprefix ../../Source/, tasks.c queue.c list.c timers.c) \
	   $(addprefix ../Common/Minimal/, PollQ.c integer.c BlockQ.c semtest.c dynamic.c flash.c) \
	   $(addprefix ParTest/, ParTest.c) \
	   $(addprefix serial/, serial.c) \
	   $(addprefix ../../Source/portable/GCC/ARM_CM4F/, port.c) \
	   $(addprefix ../../Source/portable/MemMang/, heap_2.c) \
	   $(addprefix app/src/, userbtn.c i2c.c shell.c at24c04.c spi.c) \

ASRC = $(addprefix startup/, startup_stm32f4xx.s)

LIB_CSRC = $(addprefix board/, stm32f4_discovery.c) \
			 $(addprefix lib/src/peripherals/, misc.c stm32f4xx_dma.c stm32f4xx_rcc.c stm32f4xx_adc.c stm32f4xx_gpio.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_exti.c stm32f4xx_rng.c stm32f4xx_flash.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_syscfg.c stm32f4xx_rtc.c stm32f4xx_crc.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_cryp_aes.c stm32f4xx_spi.c stm32f4xx_sdio.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_cryp.c stm32f4xx_hash.c stm32f4xx_fsmc.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_cryp_des.c stm32f4xx_hash_md5.c stm32f4xx_tim.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_cryp_tdes.c stm32f4xx_hash_sha1.c stm32f4xx_usart.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_dac.c stm32f4xx_i2c.c stm32f4xx_wwdg.c stm32f4xx_dbgmcu.c) \
			 $(addprefix lib/src/peripherals/, stm32f4xx_iwdg.c stm32f4xx_dcmi.c stm32f4xx_pwr.c stm32f4xx_can.c)

OBJS = $(ASRC:.s=.o) $(CSRC:.c=.o)

LOBJS = $(LIB_CSRC:.c=.o)

.PHONY: clean all lib flash sym dump

all:$(BIN)

$(BIN):$(OBJS) FreeRTOSConfig.h lib
	$(LINK) -o $(TGT).elf $(LINKFLAGS) $(OBJS) -L. -l$(TGT)
	$(OBJCOPY) $(OFLAGS) $(TGT).elf $@

lib:$(LOBJS)
	$(AR) -r $(LIB) $(LOBJS)

flash:$(BIN)
	$(FLASHER) $(FFLAGS)

sym:$(BIN)
	$(OBJDUMP) -t $(TGT).elf > stm32f4.sym

dump:$(BIN)
	$(OBJDUMP) -D $(TGT).elf > stm32f4.objdump

debug:$(BIN)
	$(GDBTUI) -ex "target remote localhost:3333" \
		-ex "set remote hardware-breakpoint-limit 6" \
		-ex "set remote hardware-watchpoint-limit 4" $(TGT).elf

clean:
	rm -f $(OBJS) $(LOBJS) $(TGT)* $(LIB)
