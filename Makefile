CXX = g++
CXXFLAGS = -std=c++17 -O2 -g -Wall -Wextra -Wpedantic
CPPFLAGS = -Iinclude
SOURCES := $(shell find src -name '*.cpp' -not -path 'src/ui/*' | sort)
OBJECTS := $(patsubst %.cpp,build/%.o,$(SOURCES))
TEST_SOURCES := $(wildcard tests/*Test.cpp)
TESTS := $(patsubst tests/%.cpp,build/tests/%,$(TEST_SOURCES))
.DEFAULT_GOAL := all
.PHONY: all run test clean rebuild test-id test-citizen test-random test-provider test-factory test-population test-economy test-society test-politics test-military test-disasters test-simulation
all: build/dictatorsim
build/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@
build/dictatorsim: build/apps/cli/main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
build/tests/%: build/tests/%.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
.SECONDARY:
run: all
	./build/dictatorsim
test: $(TESTS)
	@set -e; for t in $(TESTS); do echo "RUN $$t"; ./$$t; done
test-id: build/tests/IdGeneratorTest
	./$<
test-citizen: build/tests/CitizenTest
	./$<
test-random: build/tests/RandomEngineTest
	./$<
test-provider: build/tests/CSVProfessionProviderTest
	./$<
test-factory: build/tests/CitizenFactoryTest
	./$<
test-population: build/tests/PopulationManagerTest
	./$<
clean:
	rm -rf build
rebuild:
	$(MAKE) clean
	$(MAKE) all
-include $(OBJECTS:.o=.d) $(wildcard build/tests/*.d) build/apps/cli/main.d

test-economy: build/tests/ArchiveTest build/tests/ExceptionalPaymentTest build/tests/LedgerTest build/tests/AccountTest build/tests/CompanyTest build/tests/IncomeSystemTest build/tests/ConsumptionSystemTest build/tests/TaxationSystemTest build/tests/EconomyEngineTest
	./build/tests/ArchiveTest
	./build/tests/ExceptionalPaymentTest
	./build/tests/LedgerTest
	./build/tests/AccountTest
	./build/tests/CompanyTest
	./build/tests/IncomeSystemTest
	./build/tests/ConsumptionSystemTest
	./build/tests/TaxationSystemTest
	./build/tests/EconomyEngineTest

test-society: build/tests/HappinessSystemTest build/tests/CrimeSystemTest build/tests/ProtestSystemTest build/tests/RebellionSystemTest
	./build/tests/HappinessSystemTest
	./build/tests/CrimeSystemTest
	./build/tests/ProtestSystemTest
	./build/tests/RebellionSystemTest

test-politics: build/tests/PolicyExecutorTest
	./build/tests/PolicyExecutorTest

test-military: build/tests/MilitaryEngineTest
	./build/tests/MilitaryEngineTest

test-disasters: build/tests/DisasterSystemTest
	./build/tests/DisasterSystemTest

test-simulation: build/tests/SimulationClockTest build/tests/SimulationIntegrationTest
	./build/tests/SimulationClockTest
	./build/tests/SimulationIntegrationTest

LEGACY_TESTS := build/test_id_generator build/test_citizen build/test_random_engine build/test_csv_profession_provider build/test_citizen_factory build/test_population_manager
test: $(LEGACY_TESTS)
build/test_id_generator: build/tests/IdGeneratorTest
	cp $< $@
build/test_citizen: build/tests/CitizenTest
	cp $< $@
build/test_random_engine: build/tests/RandomEngineTest
	cp $< $@
build/test_csv_profession_provider: build/tests/CSVProfessionProviderTest
	cp $< $@
build/test_citizen_factory: build/tests/CitizenFactoryTest
	cp $< $@
build/test_population_manager: build/tests/PopulationManagerTest
	cp $< $@

# SFML 2.x is an optional presentation dependency; core/CLI/tests never link it.
UI_SOURCES := $(wildcard src/ui/*.cpp)
UI_OBJECTS := $(patsubst %.cpp,build/%.o,$(UI_SOURCES))
SFML_CFLAGS = $(shell pkg-config --cflags sfml-graphics 2>/dev/null)
SFML_LIBS = $(shell pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null)
.PHONY: sfml run-sfml check-sfml
check-sfml:
	@pkg-config --exists 'sfml-graphics >= 2.5' 'sfml-graphics < 3' || (echo "SFML 2.x required: sudo apt install libsfml-dev pkg-config fonts-dejavu-core"; exit 1)
$(UI_OBJECTS) build/apps/sfml/main.o: CPPFLAGS += $(SFML_CFLAGS)
$(UI_OBJECTS) build/apps/sfml/main.o: | check-sfml
build/dictatorsim-sfml: $(UI_OBJECTS) build/apps/sfml/main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(SFML_LIBS) -o $@
sfml: check-sfml build/dictatorsim-sfml
run-sfml: sfml
	./build/dictatorsim-sfml
-include $(UI_OBJECTS:.o=.d) build/apps/sfml/main.d

.PHONY: test-player
test-player: build/tests/PlayerSessionTest build/tests/PlayerDecisionImpactTest build/tests/PlayerActionsTest build/tests/EventSeverityTest
	./build/tests/PlayerSessionTest
	./build/tests/PlayerDecisionImpactTest
	./build/tests/PlayerActionsTest
	./build/tests/EventSeverityTest
	./build/tests/PlayerCoupTest

test-player: build/tests/PlayerCoupTest
