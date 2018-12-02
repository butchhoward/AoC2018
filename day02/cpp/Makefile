CXX ?= g++

# path #
SRC_PATH = src
TEST_SRC_PATH = src_test
BUILD_PATH = build
TEST_BUILD_PATH = build/test
BIN_PATH = $(BUILD_PATH)/bin
TEST_BIN_PATH = $(BUILD_PATH)/bin/test

# executable # 
BIN_NAME = find_ids
TEST_BIN_NAME = test_$(BIN_NAME)

# extensions #
SRC_EXT = cpp

# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
TEST_SOURCES = $(shell find $(TEST_SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_SRC_PATH)/%.$(SRC_EXT)=$(TEST_BUILD_PATH)/%.o)

# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)
TEST_DEPS = $(TEST_OBJECTS:.o=.d)

# flags #
COMPILE_FLAGS = -std=c++14 -Wall -Wextra -g
INCLUDES = -I include/ -I /usr/local/include

# Space-separated pkg-config libraries used by this project
LIBS =

.PHONY: default_target
default_target: all

.PHONY: dirs
dirs:
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(dir $(TEST_OBJECTS))
	@mkdir -p $(BIN_PATH)
	@mkdir -p $(TEST_BIN_PATH)

.PHONY: clean
clean:
	@$(RM) $(BIN_NAME)
	@$(RM) $(TEST_BIN_NAME)
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(TEST_BUILD_PATH)
	@$(RM) -r $(BIN_PATH)
	@$(RM) -r $(TEST_BIN_PATH)

# checks the executable and symlinks to the output
.PHONY: all
all: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
all: app test_app

app: dirs $(BIN_PATH)/$(BIN_NAME)
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

test_app: app $(TEST_BIN_PATH)/$(TEST_BIN_NAME)
	@$(RM) $(TEST_BIN_NAME)
	@ln -s $(TEST_BIN_PATH)/$(TEST_BIN_NAME) $(TEST_BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(TEST_BIN_PATH)/$(TEST_BIN_NAME): $(TEST_OBJECTS)
	$(CXX) $(TEST_OBJECTS) -o $@

# Add dependency files, if they exist
-include $(DEPS) $(TEST_DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@

$(TEST_BUILD_PATH)/%.o: $(TEST_SRC_PATH)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@