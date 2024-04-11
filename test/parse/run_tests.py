#!/usr/bin/env python3
# This is the test driver for the parser tests.
import os, sys
import subprocess
from pprint import pprint as prt
import traceback

num_run = 0
num_pass = 0
num_skip = 0
num_error = 0
num_fail = 0
num_tests = 0


def read_line(fp) :
    '''
    Read a line from the test list and skip comments and blank lines. This
    returns None when the file is ended.
    '''
    for line in fp :
        lst = line.split("#")
        line = lst[0].strip()
        if len(line) > 0 :
            yield line

def read_test_list(dir_name) :
    '''
    This creates the master test dictionary. It is created from reading the
    test_list.txt files that appear in the directories associated with the
    different parser productions. Returns a list of the lines that were
    processed.
    '''
    fname = os.path.join(dir_name, "test_list.txt")
    lst = []
    with open(fname, "r") as fp :
        for line in read_line(fp) :
            x = line.split(":")
            lst.append({'name':x[0], 'status':x[1]})

    return lst

def read_master_list() :
    '''
    Read all of the tests in from the test_list.txt files.
    '''
    lst = {}
    dir = os.getcwd()
    lst = read_test_list(dir)

    for x in lst :
        # path with the group name
        d = os.path.join(dir, x['name'])
        x['group'] = read_test_list(d)
        
        for y in x['group'] :
            p = os.path.join(d, y['name'])
            y['dir'] = p
            y['tests'] = read_test_list(p)

    #prt(lst)
    #exit(1)
    return lst

def run_test(comp, tst, dir) :
    '''
    Execute the compiler with the specified source code. Then write the result 
    to temp files. Then compare the content of the temp files to the existing 
    result files. Leading and trailing whitespace it stripped and a line-by 
    line string compare takes place. If the files are not the same, then the 
    first line is reported as an error and the test fails. When a test fails, 
    then the temp files are not deleted, but if the test passes then they are.
    '''
    global num_error, num_pass, num_fail

    tname = os.path.join(dir, tst['name']+'.simp')
    oname = os.path.join(dir, tst['name']+'.stdout')
    ename = os.path.join(dir, tst['name']+'.stderr')

    # verify that the test inputs exist
    if os.path.isfile(tname) and os.path.isfile(oname) and os.path.isfile(ename) :
        try :
            # call the compiler
            ofh = open(oname+'.temp', 'w')
            efh = open(ename+'.temp', 'w')
            pr = subprocess.run([comp, tname], text=True, stdout=ofh, stderr=efh)
            ofh.close()
            efh.close()
        except Exception :
            num_error += 1
            with open(os.path.join(dir, tst['name']+".error"), "w") as fp :
                fp.write(traceback.format_exc())
            sys.stdout.write('(ERROR) (exec)\n')

        # compare the stdout streams
        try :
            with open(oname, 'r') as fp :
                existing_file = fp.readlines()
            with open(oname+'.temp', 'r') as fp :
                temp_file = fp.readlines() 

            if len(existing_file) != len(temp_file) :
                with open(os.path.join(dir, tst['name']+".stdout.fail"), "w") as fp :
                    fp.write("file lengths differ\n")
                    if len(existing_file) == 0 :
                        fp.write("%s: zero length\n"%(oname))
                    elif len(temp_file) == 0 :
                        fp.write("%s: zero length\n\n"%(oname+'.temp'))
                    else :
                        for idx, line in enumerate(existing_file[0:min(len(existing_file), len(temp_file))]) :
                            if line.strip() != temp_file[idx].strip() :
                                fp.write("difference line: %d\n"%(idx))
                                fp.write(line.strip()+'\n')
                                fp.write(temp_file[idx].strip()+'\n')
                                break;
                num_fail == 1
                sys.stdout.write('(FAIL) (file size)\n')
                return
            else :
                for idx, line in enumerate(existing_file) :
                    if line.strip() != temp_file[idx].strip() :
                        with open(os.path.join(dir, tst['name']+".stdout.fail"), "w") as fp :
                            fp.write("difference line: %d\n"%(idx))
                            fp.write(line.strip()+'\n')
                            fp.write(temp_file[idx].strip()+'\n')
                            num_fail += 1
                            sys.stdout.write('(FAIL) (line number %d)\n'%(idx))
                            return;
        except Exception:
            num_error += 1
            with open(os.path.join(dir, tst['name']+".error"), "w") as fp :
                fp.write(traceback.format_exc())
            sys.stdout.write('(ERROR) (exec)\n')

        # compare the stderr streams
        try :
            with open(ename, 'r') as fp :
                existing_file = fp.readlines()
            with open(ename+'.temp', 'r') as fp :
                temp_file = fp.readlines() 

            for idx, line in enumerate(existing_file[0:min(len(existing_file), len(temp_file))]) :
                if line.strip() != temp_file[idx].strip() :
                    with open(os.path.join(dir, tst['name']+".stderr.error"), "w") as fp :
                        fp.write("difference line: %d"%(idx))
                        fp.write(line.strip()+'\n')
                        fp.write(temp_file[idx].strip()+'\n')
                        num_fail += 1
                        sys.stdout.write('(FAIL) (line number %d)\n'%(idx))
                        return;
        except Exception:
            num_error += 1
            with open(os.path.join(dir, tst['name']+".error"), "w") as fp :
                fp.write(traceback.format_exc())
            sys.stdout.write('(ERROR) (exec)\n')

        num_pass += 1
        sys.stdout.write('(PASS)\n')
        os.unlink(oname+'.temp')
        os.unlink(ename+'.temp')

    else :
        num_error += 1
        sys.stdout.write("(ERROR) (not found)\n")

def run_tests(comp, lst) :
    '''
    Run the list of tests for this sub-group.
    '''
    global num_tests, num_run, num_skip
    
    for item in lst['tests'] :
        num_tests += 1
        if item['status'] == 'run' :
            num_run += 1
            sys.stdout.write("\t\t'%s' -- "%(item['name']))
            run_test(comp, item, lst['dir'])
        else :
            num_skip += 1
            sys.stdout.write("\t\t'%s' -- (SKIP)\n"%(item['name']))


def skip_tests(lst) :
    '''
    Skip the list of tests for this sub-group
    '''
    global num_skip, num_tests

    for item in lst['tests'] :
        #print(item)
        sys.stdout.write("\t\t'%s' -- (SKIP)\n"%(item['name']))
        num_skip += 1
        num_tests += 1

def run_group(comp, lst) :
    for item in lst:
        if item['status'] == 'run' :
            sys.stdout.write("\t'%s' -- (RUN)\n"%(item['name']))
            run_tests(comp, item)
        else :
            sys.stdout.write("\t'%s' -- (SKIP)\n"%(item['name']))
            skip_tests(item)

def skip_group(lst) :
    for item in lst:
        sys.stdout.write("\t'%s' -- (SKIP)\n"%(item['name']))
        skip_tests(item)    
      
def runner(comp, master) :
    '''
    Run all tests that are marked with 'run'. The run flag is used to switch 
    whether a test is attempted or skipped. If the group is marked as skip, 
    all of the classes and lower branches are also disabled. This is so that 
    the test is counted whether it is run or skipped.
    '''
    for item in master :
        sys.stdout.write("\n'%s' -- "%(item['name']))
        if item['status'] == 'run' :
            sys.stdout.write("(RUN)\n")
            run_group(comp, item['group'])
        else :
            sys.stdout.write("(SKIP)\n")
            skip_group(item['group'])

def find_compiler() :
    '''
    Locate the compiler and return the fully qualified location.
    '''
    s = '../../bin/simple'
    return os.path.abspath(s)


if __name__ == "__main__" :
    
    comp = find_compiler()

    lst = read_master_list()
    runner(comp, lst)
    print("total:", num_tests,
          "-- run:", num_run, 
          "-- pass:", num_pass, 
          "-- fail:", num_fail, 
          "-- skip:", num_skip,
          "-- errors:", num_error)
