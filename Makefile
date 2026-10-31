# ==========================================
# DictatorSim - Makefile
# C++17 / GNU Make / WSL Ubuntu
# ==========================================

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g
INCLUDES = -Iinclude

# Build directory
BUILD_DIR = build

# ==========================================
# Sources
# ==========================================

SOURCES = src/tools/IdGenerator.cpp \
          src/core/RandomEngine.cpp \
          src/population/Citizen.cpp \
          src/population/CitizenFactory.cpp \
          src/population/PopulationManager.cpp \
          src/providers/CSVProfessionProvider.cpp
# ==========================================
# Executables
# ==========================================

APP = $(BUILD_DIR)/dictatorsim

# Tests
TEST_ID = $(BUILD_DIR)/test_id_generator
TEST_CITIZEN = $(BUILD_DIR)/test_citizen
TEST_RANDOM = $(BUILD_DIR)/test_random_engine
TEST_PROVIDER = $(BUILD_DIR)/test_csv_profession_provider
TEST_FACTORY = $(BUILD_DIR)/test_citizen_factory
TEST_POPULATION = $(BUILD_DIR)/test_population_manager

.PHONY: all run test test-id test-citizen test-random test-provider test-factory test-population clean rebuild
# ==========================================
# Default target
# ==========================================

all: $(APP)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ==========================================
# Main application
# ==========================================

$(APP): apps/cli/main.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# ==========================================
# Test compilation
# ==========================================

# IdGenerator
$(TEST_ID): tests/IdGeneratorTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Citizen
$(TEST_CITIZEN): tests/CitizenTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# RandomEngine
$(TEST_RANDOM): tests/RandomEngineTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# CSVProfessionProvider
$(TEST_PROVIDER): tests/CSVProfessionProviderTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# CitizenFactory tests
$(TEST_FACTORY): tests/CitizenFactoryTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@
#PopolationManger tests
$(TEST_POPULATION): tests/PopulationManagerTest.cpp $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# ==========================================
# Execution
# ==========================================

# Run simulation
run: $(APP)
	./$(APP)

# ==========================================
# Individual tests
# ==========================================

test-id: $(TEST_ID)
	./$(TEST_ID)

test-citizen: $(TEST_CITIZEN)
	./$(TEST_CITIZEN)

test-random: $(TEST_RANDOM)
	./$(TEST_RANDOM)

test-provider: $(TEST_PROVIDER)
	./$(TEST_PROVIDER)

test-factory: $(TEST_FACTORY)
	./$(TEST_FACTORY)

test-population: $(TEST_POPULATION)
	./$(TEST_POPULATION)

# ==========================================
# Run all tests
# ==========================================

test: $(TEST_ID) $(TEST_CITIZEN) $(TEST_RANDOM) $(TEST_PROVIDER) $(TEST_FACTORY) $(TEST_POPULATION)
	@echo "========== DictatorSim Tests =========="

	./$(TEST_ID)
	./$(TEST_CITIZEN)
	./$(TEST_RANDOM)
	./$(TEST_PROVIDER)
	./$(TEST_FACTORY)
	./$(TEST_POPULATION)

	@echo "========== All tests passed =========="

# ==========================================
# Maintenance
# ==========================================

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all