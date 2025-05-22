#!/bin/zsh

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Path to the miniRT executable
MINIRT_BIN="../miniRT"

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0

# Create a log directory if it doesn't exist
mkdir -p logs

# Function to check if an expected error occurs
test_error_case() {
    local test_name="$1"
    local command="$2"
    local expected_error="$3"
    
    echo "Testing: ${YELLOW}$test_name${NC}"
    
    # Run command and capture output and exit code
    output=$(eval "$command" 2>&1)
    exit_code=$?
    
    # Increment total tests
    ((total_tests++))
    
    # Log output to a file
    echo "Test: $test_name" > "logs/error_$test_name.log"
    echo "Command: $command" >> "logs/error_$test_name.log"
    echo "Exit Code: $exit_code" >> "logs/error_$test_name.log"
    echo "Output:" >> "logs/error_$test_name.log"
    echo "$output" >> "logs/error_$test_name.log"
    
    # Check if the expected error message is in the output
    if echo "$output" | grep -q "$expected_error"; then
        echo "  ${GREEN}✓ Expected error detected: '$expected_error'${NC}"
        ((passed_tests++))
    else
        echo "  ${RED}✗ Expected error not found: '$expected_error'${NC}"
        ((failed_tests++))
    fi
    
    echo ""
}

# Main test execution
echo "${YELLOW}Running miniRT error handling tests...${NC}"
echo "======================================"

# Test non-existent file
test_error_case "non_existent_file" \
                "$MINIRT_BIN non_existent_file.rt" \
                "Error: Could not open file"

# Test wrong file extension
test_error_case "wrong_extension" \
                "$MINIRT_BIN ../scenes/basic.rt.wrong" \
                "Error: File must have .rt extension"

# No arguments test
test_error_case "no_arguments" \
                "$MINIRT_BIN" \
                "Error: Invalid number of arguments"

# Too many arguments test
test_error_case "too_many_arguments" \
                "$MINIRT_BIN ../scenes/basic.rt extra_arg" \
                "Error: Invalid number of arguments"

# Print summary
echo "======================================"
echo "Error Handling Test Summary:"
echo "${GREEN}Passed: $passed_tests${NC}"
echo "${RED}Failed: $failed_tests${NC}"
echo "Total: $total_tests"

# Set exit code based on test results
if [[ $failed_tests -eq 0 ]]; then
    echo "${GREEN}All error handling tests passed!${NC}"
    exit 0
else
    echo "${RED}Some error handling tests failed. Check logs for details.${NC}"
    exit 1
fi
