NAME := factory_game
TEST_NAME := test_$(NAME)
MBC_TYPE := 0x1B
RAM_SIZE := 0x2
TEST_ENGINE_DIR := ./gb_asm_test/src
TEST_DATA_DIRECTORY := ./gb_asm_test/data
UTILS_DIR := ./gb_asm_utils
TEST_DIRECTORY := ./src/test
ADDITIONAL_INCLUDES := "-i ./src"
EMULATOR := sameboy
DEBUGGER := sameboy_debugger

build:
	@mkdir -p build
	rgbasm -i gb_asm_utils/src -i src -i data src/*.asm -o build/$(NAME).o 
	rgblink -o build/$(NAME).gb build/$(NAME).o -m build/$(NAME).map -n build/$(NAME).sym
	rgbfix -c -m $(MBC_TYPE) -r $(RAM_SIZE) -v -p 0 build/$(NAME).gb

build_test:
	make -f gb_asm_test/Makefile build_test \
	    ADDITIONAL_INCLUDES=$(ADDITIONAL_INCLUDES) \
	    TEST_ENGINE_DIR=$(TEST_ENGINE_DIR) \
	    TEST_DIRECTORY=$(TEST_DIRECTORY) \
	    TEST_NAME=$(TEST_NAME)

clean:
	rm -rf build/

test: build_test
	$(EMULATOR) build/$(TEST_NAME).gb

run: build
	$(EMULATOR) build/$(NAME).gb

debug: build
	$(DEBUGGER) build/$(NAME).gb

all: build build_test
