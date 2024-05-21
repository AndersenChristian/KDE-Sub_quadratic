BIN_DIR := ./cmake-build-debug/Google_tests
ALOI_TESTS := speed_aloi_0.01 speed_aloi_0.001 speed_aloi_0.0001 speed_aloi_0.00001

speed_aloi_all: $(ALOI_TESTS)
	@echo "running all aloi tests"

speed_aloi_0.01: $(BIN_DIR)/Speed_Tests
	@echo "TEST: Aloi 0.01"
	@./$(BIN_DIR)/Speed_Tests aloi-clean.data 0 170 0 3.3366

speed_aloi_0.001: $(BIN_DIR)/Speed_Tests
	@echo "TEST: Aloi 0.001"
	@./$(BIN_DIR)/Speed_Tests aloi-clean.data 0 2100 0 2.0346

speed_aloi_0.0001: $(BIN_DIR)/Speed_Tests
	@echo "TEST: Aloi 0.0001"
	@./$(BIN_DIR)/Speed_Tests aloi-clean.data 170 500 50 1.3300

speed_aloi_0.00001: $(BIN_DIR)/Speed_Tests
	@echo "TEST: Aloi 0.00001"
	@./$(BIN_DIR)/Speed_Tests aloi-clean.data 0 170 0


var_finder: $(BIN_DIR)/Var_Finder
	@echo "Running var test"
	@./$(BIN_DIR)/Var_Finder aloi-clean.data