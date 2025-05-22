#!/bin/zsh

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Directory containing the scenes
SCENES_DIR="../scenes"
# Path to the miniRT executable
MINIRT_BIN="../miniRT"

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0

# Create a log directory if it doesn't exist
mkdir -p logs

# Function to run a test
run_test() {
    local scene_file="$1"
    local test_name=$(basename "$scene_file")
    local should_fail=0
    
    # Determine if this test is expected to fail
    if [[ "$test_name" == "empty.rt" || "$test_name" == "invalid.rt" || "$test_name" == "alpha_numbers.rt" || 
          "$test_name" == "invalid_identifiers.rt" || "$test_name" == "unknown_identifiers.rt" || 
          "$test_name" == "fixed_invalid_identifiers.rt" || "$test_name" == "fixed_invalid_identifiers2.rt" ||
          "$test_name" == "debug_invalid_identifiers.rt" || "$test_name" == "only_invalid_identifiers.rt" ||
          "$test_name" == "test_invalid_solo.rt" || "$test_name" == "test_invalid_simple.rt" ]]; then
        should_fail=1
    fi
    
    echo "Testing: ${YELLOW}$test_name${NC}"
    
    # Run miniRT with the scene file and capture output and exit code
    output=$($MINIRT_BIN "$scene_file" 2>&1)
    exit_code=$?
    
    # Increment total tests
    ((total_tests++))
    
    # Check if the program behaved as expected
    if [[ $should_fail -eq 1 && $exit_code -ne 0 ]]; then
        echo "  ${GREEN}✓ Program correctly detected invalid input${NC}"
        ((passed_tests++))
    elif [[ $should_fail -eq 0 && $exit_code -eq 0 ]]; then
        echo "  ${GREEN}✓ Program executed successfully${NC}"
        ((passed_tests++))
    elif [[ $should_fail -eq 1 && $exit_code -eq 0 ]]; then
        echo "  ${RED}✗ Program should have failed but succeeded${NC}"
        ((failed_tests++))
    else
        echo "  ${RED}✗ Program crashed unexpectedly with exit code $exit_code${NC}"
        ((failed_tests++))
    fi
    
    # Log output to a file
    echo "Test: $test_name" > "logs/$test_name.log"
    echo "Expected to fail: $should_fail" >> "logs/$test_name.log"
    echo "Exit Code: $exit_code" >> "logs/$test_name.log"
    echo "Output:" >> "logs/$test_name.log"
    echo "$output" >> "logs/$test_name.log"
    
    # Check for error messages in a valid RT file
    if [[ "$test_name" != *"invalid"* && "$test_name" != *"empty"* && "$test_name" != *"corner"* && "$test_name" != *"edge"* && $exit_code -eq 0 ]]; then
        if echo "$output" | grep -i "error"; then
            echo "  ${YELLOW}⚠ Warning: Error message in output for valid file${NC}"
        fi
    fi
    
    echo ""
}

# Main test execution
echo "${YELLOW}Running miniRT test suite...${NC}"
echo "=============================="

# Test all scenes
for scene in "$SCENES_DIR"/*.rt; do
    run_test "$scene"
done

# Print summary
echo "=============================="
echo "Test Summary:"
echo "${GREEN}Passed: $passed_tests${NC}"
echo "${RED}Failed: $failed_tests${NC}"
echo "Total: $total_tests"

# Set exit code based on test results
if [[ $failed_tests -eq 0 ]]; then
    echo "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo "${RED}Some tests failed. Check logs for details.${NC}"
    exit 1
fi
