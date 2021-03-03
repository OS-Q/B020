# Environment Specific Flags
BUILD_FOLDER = $(CURDIR)/build/


# SDCC Compiler Specific Flags
LIBRARY = -lstm8
COMPILER_TARGET = -mstm8
OUTPUT_FORMAT = --out-fmt-ihx

# STM8FLASH Specific Flags
STM8FLASH_FOLDER = $(HOME)/tools/stm8flash/
DEBUGGER = stlinkv2
FLASH_TARGET = stm8s003?3

build: blink tim2 delay

.PHONY: blink
blink:
	mkdir -p $(BUILD_FOLDER)
	sdcc blink.c $(LIBRARY) $(COMPILER_TARGET) $(OUTPUT_FORMAT) -o$(BUILD_FOLDER)blink.ihx

.PHONY: tim2
tim2:
	mkdir -p $(BUILD_FOLDER)
	sdcc tim2_interrupt.c $(LIBRARY) $(COMPILER_TARGET) $(OUTPUT_FORMAT) -o$(BUILD_FOLDER)tim2.ihx

.PHONY: t2delay
delay:
	mkdir -p $(BUILD_FOLDER)
	sdcc tim2_delay.c $(LIBRARY) $(COMPILER_TARGET) $(OUTPUT_FORMAT) -o$(BUILD_FOLDER)delay.ihx

.PHONY: burn
burn:
	$(STM8FLASH_FOLDER)stm8flash -c $(DEBUGGER) -p $(FLASH_TARGET) -w $(BUILD_FOLDER)out.ihx

.PHONY: clean
clean:
	cd $(BUILD_FOLDER)
	rm `ls | grep -v "*.ihx" "." `


