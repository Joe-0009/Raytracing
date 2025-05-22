#!/bin/zsh

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Path to the miniRT executable
MINIRT_BIN="../miniRT"

# Scene for testing
TEST_SCENE="../scenes/basic.rt"

# Initialize counters
total_tests=0
passed_tests=0
failed_tests=0

# Create a log directory if it doesn't exist
mkdir -p logs

# Function to check if output contains expected elements
test_parser() {
    local test_name="$1"
    local expected_text="$2"
    
    echo "Testing: ${YELLOW}$test_name${NC}"
    
    # Run miniRT with the test scene
    output=$($MINIRT_BIN "$TEST_SCENE" 2>&1)
    exit_code=$?
    
    # Increment total tests
    ((total_tests++))
    
    # Log output to a file
    echo "Test: $test_name" > "logs/parser_$test_name.log"
    echo "Exit Code: $exit_code" >> "logs/parser_$test_name.log"
    echo "Output:" >> "logs/parser_$test_name.log"
    echo "$output" >> "logs/parser_$test_name.log"
    
    # Check if the expected text is in the output
    if echo "$output" | grep -q "$expected_text"; then
        echo "  ${GREEN}✓ Parser correctly identified: '$expected_text'${NC}"
        ((passed_tests++))
    else
        echo "  ${RED}✗ Parser failed to identify: '$expected_text'${NC}"
        ((failed_tests++))
    fi
    
    echo ""
}

# Main test execution
echo "${YELLOW}Running miniRT parser tests...${NC}"
echo "=============================="

# Test parsing of different elements
test_parser "ambient_light" "Ambient: ratio="
test_parser "camera" "Camera: pos="
test_parser "light" "Light"
test_parser "sphere" "Sphere"
test_parser "plane" "Plane"
test_parser "cylinder" "Cylinder"

# Print summary
echo "=============================="
echo "Parser Test Summary:"
echo "${GREEN}Passed: $passed_tests${NC}"
echo "${RED}Failed: $failed_tests${NC}"
echo "Total: $total_tests"

# Set exit code based on test results
if [[ $failed_tests -eq 0 ]]; then
    echo "${GREEN}All parser tests passed!${NC}"
    exit 0
else
    echo "${RED}Some parser tests failed. Check logs for details.${NC}"
    exit 1
fi
