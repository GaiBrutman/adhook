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
TESTS_DIR := ./tests
TESTS_BUILD_DIR := $(BUILD_DIR)/tests
TESTS_BIN_DIR := $(RELEASE_DIR)/tests
BIN_DIR := $(RELEASE_DIR)

TARGET := $(BIN_DIR)/lib$(NAME).a
															
OBJECTS :=	$(BUILD_DIR)/hook.o																\
			$(BUILD_DIR)/patch.o															\
			$(BUILD_DIR)/arch/$(ARCH)/jump.o												\
			$(BUILD_DIR)/arch/$(ARCH)/symsize.o												\

EXAMPLES := $(EXAMPLES_BIN_DIR)/patch

CFLAGS += -m32 -I$(INCLUDE_DIR) -Wall -Werror -Wno-unused-variable -Wno-unused-function
LDFLAGS := -rcs

### Tests related ###

TEST_RUNNER := $(BIN_DIR)/test_runner

GTEST_DIR ?= ./libs/googletest/googletest
# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
# Flags passed to the C++ compiler.
CXXFLAGS += -g -m32 -Wall -Wextra -pthread

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TEST_OBJECTS := $(TESTS_BUILD_DIR)/test_hook.o \
				$(TESTS_BUILD_DIR)/test_symsize.o

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

all: $(TARGET) examples test

.PHONY: examples
examples: $(TARGET) $(EXAMPLES)

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

.PHONY: test
test: $(TEST_RUNNER)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(TESTS_BUILD_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -o $@ -c $(GTEST_DIR)/src/gtest-all.cc

$(TESTS_BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -o $@ -c $(GTEST_DIR)/src/gtest_main.cc

$(TESTS_BUILD_DIR)/gtest.a : $(TESTS_BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTS_BUILD_DIR)/gtest_main.a : $(TESTS_BUILD_DIR)/gtest-all.o $(TESTS_BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(TESTS_BUILD_DIR)/%.o : $(TESTS_DIR)/%.cpp
	mkdir -p $(shell dirname $@)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(INCLUDE_DIR) -I$(SRC_DIR) $(CXXFLAGS) -o $@ -c $<

$(TEST_RUNNER): $(TEST_OBJECTS) $(OBJECTS) $(TESTS_BUILD_DIR)/gtest_main.a
	mkdir -p $(shell dirname $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET_DIR)
