#!/bin/zsh

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Change to the directory where the script is located
cd "$(dirname "$0")"

# Initialize counters
passed_suites=0
failed_suites=0
total_suites=0

# Function to run a test suite
run_test_suite() {
    local suite_name="$1"
    local suite_script="$2"
    
    echo ""
    echo "${BLUE}=== Running Test Suite: $suite_name ===${NC}"
    
    # Make the script executable
    chmod +x "$suite_script"
    
    # Run the test suite
    ./"$suite_script"
    exit_code=$?
    
    # Increment total suites
    ((total_suites++))
    
    # Check if the suite passed
    if [[ $exit_code -eq 0 ]]; then
        echo "${GREEN}✓ Test Suite Passed: $suite_name${NC}"
        ((passed_suites++))
    else
        echo "${RED}✗ Test Suite Failed: $suite_name${NC}"
        ((failed_suites++))
    fi
    
    echo ""
}

# Welcome message
echo "${YELLOW}====================================${NC}"
echo "${YELLOW}         miniRT Test Runner         ${NC}"
echo "${YELLOW}====================================${NC}"

# First check if the miniRT executable exists and is built
if [[ ! -x "../miniRT" ]]; then
    echo "${RED}Error: miniRT executable not found or not executable${NC}"
    echo "Building miniRT..."
    (cd .. && make)
    
    if [[ ! -x "../miniRT" ]]; then
        echo "${RED}Error: Failed to build miniRT${NC}"
        exit 1
    fi
fi

# Run each test suite
run_test_suite "Scene Tests" "run_tests.sh"
run_test_suite "Error Handling Tests" "test_error_handling.sh"
run_test_suite "Parser Tests" "test_parser.sh"

# Print final summary
echo "${YELLOW}====================================${NC}"
echo "Final Test Results:"
echo "${GREEN}Passed Test Suites: $passed_suites${NC}"
echo "${RED}Failed Test Suites: $failed_suites${NC}"
echo "Total Test Suites: $total_suites"

# Set exit code based on test results
if [[ $failed_suites -eq 0 ]]; then
    echo "${GREEN}All test suites passed!${NC}"
    exit 0
else
    echo "${RED}Some test suites failed. Check logs for details.${NC}"
    exit 1
fi
