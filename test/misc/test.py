# run all of the tests in this directory.
# create a log.
import argparse
import glob
from datetime import datetime
import difflib
import os, sys

# read the command line
parser = argparse.ArgumentParser(
    description='''
Run all of the tests in the test directory or run a single test. Create a log
of all of the tests run and print the results when finished. A test is a file
that is passed through the parser and the output matches the *.test file that
has the same root name as the test that is being run. The test file is
compared to the ouput of the parser and the test passes if they match and the
output file is deleted. If they don't match, then the output file is kept.
''',
epilog='The clear, and keep parameters are mutually exclusive.'
)
parser.add_argument('-s', '--stop', help='stop on errors (default is False)',
                    dest='stop', action='store_true')
parser.add_argument('-k', '--keep', help='keep passing output files (default is False)',
                    dest='keep', action='store_true')
parser.add_argument('-c', '--clear', help='clear all temp files (default is False)',
                    dest='clear', action='store_true')
parser.add_argument('-v', '--ver', help='how much to put into the log file (range is 0-10, default is 0)',
                    dest='ver', default=0, type=int)
parser.add_argument('-p', '--prog', help='specify where to find the parser program (default is "../build/simp")',
                    dest='prog', default='../build/simp')
parser.add_argument('-l', '--log', help='log file name (default is test.log)',
                    dest='log', default='test.log')
parser.add_argument('-o', '--out', help='test output file spec (default is "out")',
                    dest='out', default='out')
parser.add_argument('files', help='optional list of files with wildcards (default is "*.simp")',
                    nargs='*', metavar='files')
args = parser.parse_args()
if args.clear and args.keep:
    sys.stderr.write("command error: clear and keep are mutually exclusive.\n\n")
    parser.print_help()

# make lists of all the files
if len(args.files) == 0:
    infiles = glob.glob('*.simp')
else:
    infiles = args.files
outfiles = []
tmpfiles = []
for idx, name in enumerate(infiles):
    ex = name.split('.')
    if len(ex) < 2:
        ex.append('simp')
    outfiles.append('.'.join([ex[0], args.out]))
    tmpfiles.append('.'.join([ex[0], 'tmp']))

# verify that the test program exists
prog = os.path.realpath(args.prog)
if not os.path.isfile(prog):
    sys.stderr.write("fatal error: parser program '"+prog+"' cannot be located.\n")
    exit(1)

total_tests = 0
total_fails = 0

with open(args.log, "w") as ofile:
    if args.clear:
        for name in tmpfiles:
            os.remove(name)
    else:
        ofile.write("\n------------------------------------------\n")
        ofile.write("test in         = %s\n"%(os.path.dirname(os.path.realpath(__file__))))
        ofile.write("test started on = %s\n"%(str(datetime.now())))
        if args.ver > 4:
            print("test in         = %s"%(os.path.dirname(os.path.realpath(__file__))))
            print("test started on = %s"%(str(datetime.now())))
        if args.ver > 4:
            ofile.write("infiles = %s\n\n"%(str(infiles)))
            ofile.write("outfiles = %s\n\n"%(str(outfiles)))
        if args.ver > 9:
            print("infiles = %s\n"%(str(infiles)))
            print("outfiles = %s\n"%(str(outfiles)))
        ofile.write("------------------------------------------\n\n")
        if args.ver > 1:
            print("------------------------------------------\n")

        num_files = 1
        for idx, name in enumerate(infiles):
            logs = []
            logs.append("%d. testing: %s: "%(num_files, name))
            os.system("%s %s > %s 2>&1"%(prog, name, tmpfiles[idx]))
            fails = 0
            with open(tmpfiles[idx], 'r') as left:
                with open(outfiles[idx], 'r') as right:
                    left_lines = left.readlines()
                    right_lines = right.readlines()
                    ds = difflib.ndiff(left_lines, right_lines)
                    for line in ds:
                        logs.append("        %s"%(line[3:]))
                        fails += 1

            if fails == 0:
                if not args.keep:
                    os.remove(tmpfiles[idx])
                logs[0] = logs[0] + "PASS\n"
                if args.ver > 4:
                    print("test: %s PASS"%(name))
            else:
                total_fails += 1
                logs[0] = logs[0] + "FAIL\n"
                if args.ver > 1:
                    print("test: %s FAIL"%(name))
                if args.stop:
                    break

            for line in logs:
                ofile.write(line)

            total_tests += 1
            num_files += 1

        ofile.write("\n------------------------------------------\n")
        ofile.write("test ended on = " + str(datetime.now()) + '\n')
        ofile.write("tests run     = %d\n"%(total_tests))
        ofile.write("tests passed  = %d\n"%(total_tests-total_fails))
        ofile.write("tests failed  = %d\n\n\n"%(total_fails))

if args.ver > 1:
    print("\n------------------------------------------\n")
if args.ver > 0:
    print("tests run     = %d"%(total_tests))
    print("tests passed  = %d"%(total_tests-total_fails))
    print("tests failed  = %d\n"%(total_fails))
