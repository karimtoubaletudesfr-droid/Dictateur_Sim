CXX =  g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g 
INCLUDES= -Iinclude

BUILD_DIR= build

SOURCES = src/tools/IdGenerator.cpp

APP = $(BUILD_DIR)/dictatorsim

TEST = $(BUILD_DIR)/test_id_generator

.PHONY: all run test clean

all: $(APP)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


$(APP): apps/cli/main.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@


$(TEST): tests/IdGeneratorTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

run: $(APP)
	./$(APP)


test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD_DIR)