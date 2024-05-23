# C编译器的宏定义
C_DEFS = \
-DN32L40X \
-DUSE_STDPERIPH_DRIVER

# C编译器的头文件搜索路径
INCLUDE =  \
-ICMSIS/core  \
-ICMSIS/device	\
-Istd_periph_lib/inc \
-Iuser \
-Imsp \
-Idriver/inc \
-Iapplication/inc \
-Icomponents/letter_shell \
-Icomponents/soft_timer \
-Icomponents/cm_backtrace

# APP源文件
C_APP_SOURCES =	\
application/main.c \
application/n32l40x_it.c \
user/shell_debug.c

# 中间层驱动源文件
C_DRV_SOURCES =  \
msp/board.c \
msp/drv_msp.c \
driver/src/drv_adc.c \
driver/src/drv_flash.c \
driver/src/drv_gpio.c \
driver/src/drv_iwdg.c \
driver/src/drv_pwm_gpio*.c \
driver/src/drv_pwm_input*.c \
driver/src/drv_spi*.c \
driver/src/drv_usart*.c \
$(wildcard components/letter_shell/*.c) \
components/soft_timer/soft_timer.c \
components/cm_backtrace/cm_backtrace.c \
components/cm_backtrace/fault_test.c

# C编译器的底层驱动源文件
C_LIB_SOURCES =  \
CMSIS/device/system_n32l40x.c \
std_periph_lib/src/n32l40x_rcc.c \
std_periph_lib/src/n32l40x_tim.c \
std_periph_lib/src/n32l40x_adc.c \
std_periph_lib/src/n32l40x_dma.c \
std_periph_lib/src/n32l40x_usart.c \
std_periph_lib/src/n32l40x_iwdg.c \
std_periph_lib/src/n32l40x_flash.c \
std_periph_lib/src/n32l40x_gpio.c \
std_periph_lib/src/n32l40x_exti.c \
std_periph_lib/src/n32l40x_spi.c \
std_periph_lib/src/misc.c

C_SOURCES := $(C_LIB_SOURCES)
C_SOURCES += $(C_APP_SOURCES)
C_SOURCES += $(C_DRV_SOURCES)

ASM_SOURCES =  \
CMSIS/device/startup/startup_n32l40x_gcc.s

ASM_SOURCES2 =  \
components/cm_backtrace/fault_handler/gcc/cmb_fault.S

# 指定交叉编译工具链的 编译器、连接器、库管理器、
CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size

# 指定目标硬件的架构和浮点运算单元
MCU = -mcpu=cortex-m4 -mthumb \
	-ffunction-sections \
	-fdata-sections \
	--specs=nosys.specs \
	--specs=nano.specs \
	-Os -ggdb

# 定义编译C源文件的编译选项：禁用共享变量、函数和数据分段、优化级别、调试信息级别、开启所有警告、目标架构、预定义宏
# O0:禁用优化，O1：基本优化，O2, O3, Os
# g0:禁用调试信息, g1, g2, g3, g				
CFLAGS= -c -fno-common \
	--specs=rdimon.specs \
	-std=gnu99 -mabi=aapcs \
	-Wall \
	$(MCU) \
	$(C_DEFS)

# 定义链接器脚本、链接选项、目标文件格式转换选项、反汇编
LDSCRIPT=n32l40x_flash.ld
LDFLAGS = -Wl,--gc-sections --data-sections -mabi=aapcs $(MCU) -T$(LDSCRIPT) \
	-x assembler-with-cpp -Wa,-mimplicit-it=thumb
OCFLAGS	= -Obinary
ODFLAGS	= -S

#输出目录和目标文件名
OUTPUT_DIR = build
TARGET  = $(OUTPUT_DIR)/app

# C源文件、汇编源文件的目标文件路径
C_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(C_SOURCES:.c=.o))
ASM_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(ASM_SOURCES:.s=.o))
ASM_OBJECTS2 = $(addprefix $(OUTPUT_DIR)/, $(ASM_SOURCES2:.S=.o))

.PHONY : clean all

all: $(TARGET).bin $(TARGET).list $(TARGET).hex
	$(SZ) $(TARGET).elf
	@make copy

.PHONY: copy download

copy: $(TARGET).bin
	cp $(TARGET).bin app.bin
	cp $(TARGET).hex app.hex

clean:
	rm -rf $(OUTPUT_DIR)
	rm app.bin
	rm app.hex

SYS := $(shell uname -a)

ifeq ($(findstring Microsoft,$(SYS)),Microsoft)
COPY_CMD:
	cp app.hex "/mnt/c/Users/Breo/Desktop/iap-tools/linux_download/"
	wsl.exe -d Ubuntu-20.04 cmd.exe /c "C:\\\Users\\\BREO\\\Desktop\\\iap-tools\\\linux_download\\\program.bat"
else
COPY_CMD:
	echo "当前系统不是 WSL，跳过拷贝文件指令"
endif

download:
	@make all
	@$(MAKE) COPY_CMD

# 定义生成目标文件的规则
$(TARGET).list: $(TARGET).elf
	$(OD) $(ODFLAGS) $< > $(TARGET).lst

$(TARGET).bin: $(TARGET).elf
	$(OC) $(OCFLAGS) $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(ASM_OBJECTS) $(ASM_OBJECTS2) $(C_OBJECTS)
	$(CC) $(ASM_OBJECTS) $(ASM_OBJECTS2) $(C_OBJECTS) $(LDFLAGS) -Wl,-Map=$(TARGET).map -o $(TARGET).elf

#$(C_OBJECTS): $(C_SOURCES)

$(TARGET).hex: $(TARGET).elf
	$(OC) -O ihex $< $@

# 定义编译C源文件、汇编源文件的规则，将源文件编译成目标文件
$(OUTPUT_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(CFLAGS) -MMD -c $< -o $@

$(OUTPUT_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@


-include $(wildcard $(OUTPUT_DIR)/**/*.d)	# 包含所有生成的依赖文件，避免重复编译、提高效率
-include $(wildcard $(OUTPUT_DIR)/**/*/*.d)	# 包含所有生成的依赖文件，避免重复编译、提高效率
