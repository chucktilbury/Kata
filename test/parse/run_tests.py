# This is the test driver for the parser tests.
import os, sys
import shutil
from pprint import pprint as prt

num_run = 0
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

    lst = {}
    dir = os.getcwd()
    lst = read_test_list(dir)

    for x in lst :
        #print('x', x)
        d = os.path.join(dir, x['name'])
        x['class'] = read_test_list(d)

        for y in x['class'] :
            #print("y", y)
            y['dir'] = os.path.join(dir, d, y['name'])
            p = os.path.join(y['dir'], 'correct')
            y['correct'] = read_test_list(p)

        for y in x['class'] :
            #print("y", y)
            p = os.path.join(dir, d, y['name'], 'errors')
            y['errors'] = read_test_list(p)

    return lst

def run_test(name) :
    '''
    Run the specified test
    '''
    print(name)



def runner(master) :
    '''
    Run all tests that are marked with an 'r'.
    '''

    global num_run, num_skip
    for item in master :

        if item['status'].lower() == 'r' :
            for cls in item['class'] :
                if cls['status'].lower() == 'r' :

                    for tst in cls['correct'] :
                        run_test(os.path.join(cls['dir'], 'correct', tst['name']))
                        num_run += 1

                    for tst in cls['errors'] :
                        run_test(os.path.join(cls['dir'], 'errors', tst['name']))
                        num_run += 1
                else :
                    print("skip class:", cls['name'])
                    num_skip += len(cls['correct'])
                    num_skip += len(cls['errors'])

        else :
            print("skip group:", item['name'])
            num_skip += len(item['class']['correct'])
            num_skip += len(item['class']['errors'])



if __name__ == "__main__" :

    runner(read_master_list())
    print("run:", num_run, "skip:", num_skip)
