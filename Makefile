NAME := adhook

MODE ?= debug

LD := $(CC)
LDFLAGS := 

CFLAGS += -flto -Os

TARGET_DIR := ./target
RELEASE_DIR := $(TARGET_DIR)/$(MODE)
SRC_DIR := ./src
BUILD_DIR := $(RELEASE_DIR)/build
BIN_DIR := $(RELEASE_DIR)
TARGET := $(BIN_DIR)/adhook

OBJECTS := $(BUILD_DIR)/main.o

all: $(TARGET_DIR) $(RELEASE_DIR) $(BUILD_DIR) $(TARGET)

$(TARGET_DIR):
	mkdir $@
$(RELEASE_DIR):
	mkdir $@
$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^
	strip -SrXx $@ -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET_DIR)
