# Makefile for miniRT tests

# Set test scripts
TEST_FILES = run_tests.sh test_error_handling.sh test_parser.sh
TEST_RUNNER = run_all_tests.sh

# Default target
all: run

# Make scripts executable
prepare:
	@chmod +x $(TEST_RUNNER) $(TEST_FILES)

# Run all tests
run: prepare
	@./$(TEST_RUNNER)

# Clean logs
clean:
	@rm -rf logs/*.log
	@echo "Cleaned test logs"

# Clean and run the tests
re: clean run

.PHONY: all prepare run clean re
