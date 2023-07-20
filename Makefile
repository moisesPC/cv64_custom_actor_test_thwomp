#### Files and directories ####
BUILD = build
SRC_DIR = src
SRC_NAME = thwomp
ASSETS_NAME = assets
MAP_NAME = map
OVERLAY_OUT = ovl
PATCH_ROM = cv64_custom_actor_test.z64

### ROM injection offsets ###
ASSETS_INJECT_OFFSET = BB2D88
OVERLAY_INJECT_OFFSET = BB6D88
MAP_INJECT_OFFSET = BBAD88

### File IDs ###
ASSETS_FILE_ID = 1B
OVERLAY_FILE_ID = 82
MAP_FILE_ID = 5C

#### Tools ####
# Compiler
CC   = tools/ido71/cc
# Linker
LD   = mips64-ld
# nOVL (ELF -> overlay conversion)
NOVL = tools/nOVL/novl
# Used to convert textures from .PNG -> .C
ZAPD = tools/ZAPD.out
# Python
PYTHON = python3
# Patch programs
PATCH = tools/cv64_file_insert.py
MISC_PATCHES = tools/cv64_misc_patches.py

#### Options ####
# Includes
INCLUDES := -Iinclude -Iinclude/game -Iinclude/mod -Iinclude/ultra64

# Compiler flags
CFLAGS = -G0 -mips2 -non_shared -fullwarn -verbose -Xcpluscomm -Wab,-r4300_mul $(INCLUDES)
OPTFLAGS := -O2

#### Thwomp model and $(MAP_NAME) texture files ####
TEXTURES_ASSETS_PNG := $(wildcard $(ASSETS_NAME)/*.png)
TEXTURES_ASSETS_C    = $(patsubst %.png, %.c, $(TEXTURES_ASSETS_PNG))
TEXTURES_MAP_PNG := $(wildcard $(MAP_NAME)/*.png)
TEXTURES_MAP_C    = $(patsubst %.png, %.c, $(TEXTURES_MAP_PNG))

#### Recipes ####

default: all
all: patch

patch: $(OVERLAY_OUT)
	$(PYTHON) $(PATCH) $(PATCH_ROM) $(BUILD)/$(ASSETS_NAME)/$(ASSETS_NAME).bin $(ASSETS_INJECT_OFFSET) $(ASSETS_FILE_ID)
	$(PYTHON) $(PATCH) $(PATCH_ROM) $(BUILD)/$(OVERLAY_OUT).bin $(OVERLAY_INJECT_OFFSET) $(OVERLAY_FILE_ID)
	$(PYTHON) $(PATCH) $(PATCH_ROM) $(BUILD)/$(MAP_NAME)/$(MAP_NAME).bin $(MAP_INJECT_OFFSET) $(MAP_FILE_ID)
	$(PYTHON) $(MISC_PATCHES) $(PATCH_ROM)

$(OVERLAY_OUT): $(ASSETS_NAME) $(MAP_NAME)
	mkdir -p $(BUILD)
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $(BUILD)/$(OVERLAY_OUT).o $(SRC_DIR)/$(SRC_NAME).c
	$(LD) -o $(BUILD)/$(OVERLAY_OUT).elf $(BUILD)/$(OVERLAY_OUT).o -T linker/$(SRC_DIR)/linker.ld
	$(NOVL) -c -o $(BUILD)/$(OVERLAY_OUT).bin $(BUILD)/$(OVERLAY_OUT).elf

$(ASSETS_NAME): $(TEXTURES_ASSETS_C)
	mkdir -p $(BUILD)/$(ASSETS_NAME)
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $(BUILD)/$(ASSETS_NAME)/$(ASSETS_NAME).o $(ASSETS_NAME)/$(ASSETS_NAME).c
	$(LD) -o $(BUILD)/$(ASSETS_NAME)/$(ASSETS_NAME).elf $(BUILD)/$(ASSETS_NAME)/$(ASSETS_NAME).o -T linker/$(ASSETS_NAME)/linker.ld
	$(NOVL) -c -o $(BUILD)/$(ASSETS_NAME)/$(ASSETS_NAME).bin $(BUILD)/$(ASSETS_NAME)/$(ASSETS_NAME).elf

$(MAP_NAME): $(TEXTURES_MAP_C)
	mkdir -p $(BUILD)/$(MAP_NAME)
	$(CC) -c $(CFLAGS) $(OPTFLAGS) -o $(BUILD)/$(MAP_NAME)/$(MAP_NAME).o $(MAP_NAME)/$(MAP_NAME).c
	$(LD) -o $(BUILD)/$(MAP_NAME)/$(MAP_NAME).elf $(BUILD)/$(MAP_NAME)/$(MAP_NAME).o -T linker/$(MAP_NAME)/linker.ld
	$(NOVL) -c -o $(BUILD)/$(MAP_NAME)/$(MAP_NAME).bin $(BUILD)/$(MAP_NAME)/$(MAP_NAME).elf

%.c: %.png
	mkdir -p $(BUILD)/temp
	$(ZAPD) btex -eh -tt $(subst .,,$(suffix $*)) -i $< -o $@
	mv $@ $(BUILD)/temp

clean:
	rm -rf $(BUILD)
