# Parser Tests
This directory holds tests for the parser.

This test is designed to test the whole parser, including parsing correct and incorrect syntax. It is based on the grammar productions as implemented by the parser. All of the tests that are not commented out are run and the results of the run are printed to stdout.

## Running the tests:
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
