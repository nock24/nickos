GNU := aarch64-linux-gnu

C_FLAGS := -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASM_FLAGS := -Iinclude

SRC_DIR := src
LINKER_SCRIPT := linker.ld
BUILD_DIR := build
KERNEL := $(BUILD_DIR)/kernel.img

default: $(KERNEL)

clean:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(GNU)-gcc $(C_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(GNU)-gcc $(ASM_FLAGS) -MMD -c $< -o $@

C_FILES := $(wildcard $(SRC_DIR)/*.c)
C_FILES += $(wildcard $(SRC_DIR)/*/*.c)
ASM_FILES := $(wildcard $(SRC_DIR)/*.S)
ASM_FILES += $(wildcard $(SRC_DIR)/*/*.S)

OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES := $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

$(KERNEL): $(OBJ_FILES) 
	$(GNU)-ld -T $(LINKER_SCRIPT) -o $(BUILD_DIR)/kernel.elf $(OBJ_FILES)
	$(GNU)-objcopy $(BUILD_DIR)/kernel.elf -O binary $@

qemu: $(KERNEL)
	qemu-system-aarch64 -m 1G -M raspi3b -kernel $(KERNEL) -serial null -serial stdio
