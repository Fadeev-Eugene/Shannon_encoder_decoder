CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS := -lgtest -lgtest_main -pthread

SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build

SRC := $(SRC_DIR)/main.cpp $(SRC_DIR)/shannon_encoder.cpp $(SRC_DIR)/shannon_decoder.cpp
TEST_SRC := $(TEST_DIR)/test_shannon.cpp $(SRC_DIR)/shannon_encoder.cpp $(SRC_DIR)/shannon_decoder.cpp

.PHONY: all clean test shannon tests

all: shannon tests

shannon: $(BUILD_DIR)/shannon

tests: $(BUILD_DIR)/shannon_tests

$(BUILD_DIR)/shannon: $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/shannon_tests: $(TEST_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

test: $(BUILD_DIR)/shannon_tests
	@./$(BUILD_DIR)/shannon_tests

clean:
	@rm -rf $(BUILD_DIR)
