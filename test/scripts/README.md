# Test scripts
This directory contains scripts used to run tests. To run tests, it is assumed that you have run the setup script in the project root directory. That script creates the build directory if it doesn't exist and sets up the executable path to include this directory and the directory where the compiler exists.

## Parser tests
The parser tests verify that the parser accepts correct syntax and rejects incorrect syntax. The tests in that directroy are not really intended to make symantic sense. The parser tests also include the AST traverse functions. This is because they are closely associated and a bug in one is likely to produce a bug in the other. To run these tests, the compiler must be built with the USE_TRACE name defined. This produces a very large amount of console output because every parser and ast traverse function at least prints ENTER and RET messages so that the developer can verify that the sequence of events is correct for the input. This can get very complicated and it's also used to verify that the grammar is correct and means what we think it means.

### File format of the ```test_list.txt``` file
The test suite operates with a tree structure of groups where the leaves of the tree are the actual tests. A group is implemented as a directory. Every directory (AKA group) has a file called ```test_list.txt```. This file has a list of items in it that describe the group. Comments are allowed in the file starting with a ```'#'``` character and extending to the end of the line. (just like Python)

#### Valid group items
* ```verbosity``` 
  * This specifies the level of output that the group will produce. Valid values are numbers 0 through 10. Other value produce an error and no tests will be run. This value applies to the items in the group, and not to the group itself. No tests or directories can be named "verbosity".
  * Example
    * ```verbosity:10```
* ```testname```
  * This specifies the arbitrary name of a test. A normal test name has no dot ```'.'```. There are several file extensions needed to run a test and they are added automatically by the test suite. Valid values for a test name are ```run``` and ```skip```. This name can name a directroy as well as an actual file name. If a directory is named, then a ```test_list.txt``` file is expected to be found in it. If not, then an error is created and no tests are run. If a file is named, then the contents of the file are expected to be a test for the **Kata** parser. If there is a file in the directory or a directroy that is not named in the ```test_list.txt``` file, then it is ignored as if it does not exist.
  * Examples
    * ```testname:run```
    * ```another:skip```

### Create a test
* Switch to the directory that will contain the test. This directory must have an existing ``test_list.txt`` file.
* Run the command ```addt testname```. This will create all of the stub files needed to run the test and add it to the ``test_list.txt`` file.
* Edit the ``testname.k`` file so that it contains the syntax that you want to test. Keep it simple. It's a lot.
* Run the compiler as ```kata testname``` and go over the output, line by line, to verify that it is correct. Note that a test can have syntax errors in it to verify that correct errors are being published when they should be.
* After verifying that the compiler outputs what is expected, then run the test suite in the directory as ```runt```. This will create the files needed to certify the test for future runs.
* Lastly, certify the test by the command ```cert testname```. The test is now a part of the test suite.

### Running the tests
The test driver ```runt``` expects to have a ```test_list.txt``` file in the current directory. That file will be read as if it is a group of tests. See the file format above. 
* Simply change the to directory with the tests and run ```runt```.
* From the build directory run ```make parse_tests``` and all of the tests will be run.
