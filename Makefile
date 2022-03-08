NAME := adhook

MODE ?= debug

ARCH ?= i386

LD := $(CC)
AR := ar

TARGET_DIR := ./target
RELEASE_DIR := $(TARGET_DIR)/$(MODE)
SRC_DIR := ./src
INCLUDE_DIR := ./include
EXAMPLES_DIR := ./examples
BUILD_DIR := $(RELEASE_DIR)/build
EXAMPLES_BIN_DIR := $(RELEASE_DIR)/examples
BIN_DIR := $(RELEASE_DIR)

TARGET := $(BIN_DIR)/lib$(NAME).a
															
OBJECTS :=	$(BUILD_DIR)/hook.o																\
			$(BUILD_DIR)/patch.o															\
			$(BUILD_DIR)/arch/$(ARCH)/jump.o												\
			$(BUILD_DIR)/arch/$(ARCH)/symsize.o												\

EXAMPLES := $(EXAMPLES_BIN_DIR)/patch

CFLAGS += -m32 -I$(INCLUDE_DIR)
LDFLAGS := -rcs

all: $(TARGET) examples

.PHONY: examples
examples: $(EXAMPLES)

$(EXAMPLES_BIN_DIR)/% : $(EXAMPLES_DIR)/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $< -L$(BIN_DIR) -l$(NAME)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $@ -c $<

$(TARGET): $(OBJECTS)
	mkdir -p $(shell dirname $@)
	$(AR) $(LDFLAGS) $@ $^

ifneq ($(MODE), debug)
	strip -sXx $@ -o $@
endif

.PHONY: clean
clean:
	rm -rf $(TARGET_DIR)
