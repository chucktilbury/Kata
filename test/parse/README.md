# Parser Tests
This directory holds tests for the parser. The purpose of these tests is to verify that the parser accepts correct input and rejects incorrect input.

This test is designed to test the whole parser, including parsing correct and incorrect syntax. It is based on the grammar productions as implemented by the parser. All of the tests that are not commented out are run and the results of the run are printed to stdout.

## Running the tests:
Provisions are made for the verbosity of test execution as well as running tests singularly, by class, and by group. Tests are intended to be run manually.

### Running a test:
* run ```./setup debug``` from the root directory to set up a debug build.
* type ```make``` to build the compiler.
* type ```python run_tests.py``` and observe the result.
* to save the result to a file, then redirect using the shell.

## Possible test outcomes
* PASS - means that the expected results were produced by the parser.
* FAIL - means that the results produced by the parser were unexpected.
* SKIP - means that the test was skipped because it was "commented out"
* ERROR - means that the test could not be run because of an error in the test

## Verbosity
Each higher level reports more information than the previous level. The highest level produces the greatest amount of output.
* 0 - Only errors are reported.
* 1 - Only "PASS" or "FAIL" is reported after the end of the test. This is the default. If "FAIL" is reported, then the number of individual tests that failed are reported.
* 2 - Simple statistics are given at the end of the test.
* 3 - Fail messages are given.
* 4 - Pass and fail messages are given.
* 5 - Print every test name with pass, fail, and skipped messages.
* >10 - Print debugging info as test harness runs.a

## How tests work
The purpose of these tests is to verify that the parser accepts correct input and rejects incorrect input. Accordingly, input is designed to exercise specific parts of the parser to verify its operation. The most natural way to do that is to organize the tests by grammar production, since the parser is designed around the grammar. The grammar productions are placed in files that roughly organizes them by what they do. There are functions in these files that implement the actual parsing of input. These tests are arranged in the same manner.

All of these tests assume that the compiler was built with debugging and with trace turned on.

Tests follow these steps:
* If the specific test has a ":r" after the name, then it is intended to run. If it has any other character then the test is not run. Tests can also be disabled by group and class.
* Run the compiler against a specific source and capture the stdout output into a file.
* Compare the compiler output with another file that has been hand-verified to be correct.
* If the files match, then the test passes and the captured output is deleted.
* If the files do not match, then the test fails and the captured output is not deleted.
* If the capture is not found, then the test is an error.
