# miniRT Testing Infrastructure

This directory contains automated tests for the miniRT ray tracer project.

## Test Structure

The testing system is organized as follows:

- `run_all_tests.sh`: Master script that runs all test suites
- `run_tests.sh`: Tests all scene files to ensure the program handles them correctly
- `test_error_handling.sh`: Tests error handling for invalid inputs
- `test_parser.sh`: Tests the scene parsing functionality
- `Makefile`: Provides targets to run the tests

## Running the Tests

### Run all tests
```
make
```

### Clean logs and run tests
```
make re
```

### Clean logs only
```
make clean
```

## Test Logs

All test logs are stored in the `logs/` directory. Each test generates a log file with the name of the test and contains:

- The test name
- The command executed
- The exit code
- The output from the program

## Adding New Tests

To add new tests:

1. Create a new test file (e.g., `test_new_feature.sh`)
2. Add the file to the list of test files in the Makefile
3. Follow the pattern of the existing test files:
   - Define tests with clear names
   - Check for expected output
   - Log results
   - Provide a summary

## Test Scenes

The test system uses scene files from the `../scenes/` directory:

- `basic.rt`: A simple scene with basic elements
- `advanced.rt`: A more complex scene
- `corner_cases.rt`: Tests corner cases
- `edge_cases.rt`: Tests edge cases
- `empty.rt`: An empty scene file
- `extreme_values.rt`: Tests with extreme values
- `invalid.rt`: An invalid scene file

## Extending the Testing System

To extend the testing system:

1. Add more test files to test specific functionality
2. Add more test scenes to cover additional cases
3. Enhance the test scripts to check for more detailed behavior
