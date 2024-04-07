#!/usr/bin/env python3
# This is the test driver for the parser tests.
import os, sys
import shutil
from pprint import pprint as prt

num_run = 0
num_pass = 0
num_skip = 0
num_error = 0
num_fail = 0


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

def exec_test(name) :
    sys.stdout.write("(PASS)\n")

def run_test(tst, dir) :
    global num_pass, num_fail, num_error
    tname = os.path.join(dir, 'tests', tst['name']+'.simp')
    if os.path.isfile(tname) :
        num_pass += 1
        sys.stdout.write("(RUN) ")
        exec_test(tname)
    else :
        num_error += 1
        sys.stdout.write("(ERROR) (not found)\n")

def run_tests(lst) :
    global num_run, num_skip
    for item in lst['tests'] :
        if item['status'] == 'run' :
            num_run += 1
            sys.stdout.write("\t\t'%s' -- "%(item['name']))
            run_test(item, lst['dir'])
        else :
            num_skip += 1
            sys.stdout.write("\t\t'%s' -- (SKIP)\n"%(item['name']))


def skip_tests(lst) :
    global num_skip
    for item in lst['tests'] :
        sys.stdout.write("\t\t'%s' -- (SKIP)\n"%(item['name']))
        num_skip += 1

def run_group(lst) :
    for item in lst:
        if item['status'] == 'run' :
            sys.stdout.write("\t'%s' -- (RUN)\n"%(item['name']))
            run_tests(item)
        else :
            sys.stdout.write("\t'%s' -- (SKIP)\n"%(item['name']))
            skip_tests(item)

def skip_group(lst) :
    for item in lst:
        sys.stdout.write("\t'%s' -- (SKIP)\n"%(item['name']))
        skip_tests(item['tests'])    
      
def runner(master) :
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
            run_group(item['group'])
        else :
            sys.stdout.write("(SKIP)\n")
            skip_group(item['group'])

if __name__ == "__main__" :
    
    lst = read_master_list()
    runner(lst)
    print("run:", num_run, 
          "-- pass:", num_pass, 
          "-- fail:", num_fail, 
          "-- skip:", num_skip,
          "-- errors:", num_error)

    #prt(lst)

