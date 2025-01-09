GNU := aarch64-linux-gnu

C_FLAGS := -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ZIG_FLAGS := -target aarch64-linux-gnu -O ReleaseSmall -fstrip -Iinclude
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

$(BUILD_DIR)/%_zig.o: $(SRC_DIR)/%.zig
	mkdir -p $(@D)
	zig build-obj $< $(ZIG_FLAGS) -femit-bin=$@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(GNU)-gcc $(ASM_FLAGS) -MMD -c $< -o $@

C_FILES := $(shell find $(SRC_DIR) -name '*.c')
ZIG_FILES := $(shell find $(SRC_DIR) -name '*.zig')
ASM_FILES := $(shell find $(SRC_DIR) -name '*.S')

OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%_c.o,$(C_FILES))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.zig,$(BUILD_DIR)/%_zig.o,$(ZIG_FILES))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%_s.o,$(ASM_FILES))

DEP_FILES := $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

$(KERNEL): $(OBJ_FILES) 
	$(GNU)-ld -T $(LINKER_SCRIPT) -o $(BUILD_DIR)/kernel.elf $(OBJ_FILES)
	$(GNU)-objcopy $(BUILD_DIR)/kernel.elf -O binary $@

qemu: $(KERNEL)
	qemu-system-aarch64 -m 1G -M raspi3b -drive file=./disk.img,if=sd,format=raw -kernel $(KERNEL) -serial stdio
