BIN_DIR := ./cmake-build-debug/Google_tests

speed_test_aloi: $(BIN_DIR)/Speed_Tests
	@echo "Running speed test"
	@./$(BIN_DIR)/Speed_Tests aloi-clean.data
