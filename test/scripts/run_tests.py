#!/usr/bin/env python3
# This is the test driver for the parser tests.
import os, sys
import subprocess
from pprint import pprint as prt
import traceback

class Stack:

    def __init__(self, value=None):
        self.stack = []
        if not value is None:
            self.stack.append(value)

    def push(self, value):
        #print('PUSH(%d)'%(value))
        self.stack.append(value)

    def pop(self):
        #print('POP()')
        return self.stack.pop()
    
    def peek(self):
        return self.stack[-1]
    
class Message:

    def __init__(self, level=10, stream=sys.stderr, increment=4):
        self.verbosity = Stack(value=int(level))
        self.stream = stream
        self.count = 0
        self.increment = increment

    def write(self, level, msg):
        if self.verbosity.peek() >= level:
            if msg[0] != ' ':
                self.stream.write(' '*self.count + msg)
            else:
                self.stream.write(msg)

    def push(self, level):
        self.verbosity.push(level)

    def pop(self):
        return self.verbosity.pop()
    
    def peek(self):
        return self.verbosity.peek()

    def add_count(self):
        self.count += self.increment

    def sub_count(self):
        self.count -= self.increment

    def get_count(self):
        return self.count

class Compiler:
    '''
    Manage the compiler.
    '''

    def __init__(self):
        '''
        Find the compiler and get it ready to run tests.
        '''
        self.comp_name = self.find_compiler()
        self.msg = Message()

    def find_compiler(self):
        '''
        Stub
        '''
        return 'kata'
    
    def run(self, name):
        '''
        Run the compiler and catch errors that relate to it. Return a string 
        that describes the result. If the string is 'OK' then there was no 
        error. Any other string describes the error that happened.
        '''
        oname = name+'.stdout'
        ename = name+'.stderr'

        # verify that the test inputs exist
        if not os.path.isfile(name+'.k'):
            return "the test file '%s' does not exist"%(name+'.k')
        # create the temp files if they don't exist
        if not os.path.isfile(oname): 
            with open(oname, 'w') as fp:
                pass
        if not os.path.isfile(ename):
            with open(ename, 'w') as fp:
                pass

        try :
            # call the compiler
            ofh = open(oname+'.temp', 'w')
            efh = open(ename+'.temp', 'w')
            pr = subprocess.run([self.comp_name, name+'.k'], text=True, stdout=ofh, stderr=efh)
            ofh.close()
            efh.close()
            if pr.returncode != 0:
                return "compiler returned error code %d"%(pr.returncode)
            
        except Exception as e:
            with open(os.path.join(dir, name+".error"), "w") as fp :
                fp.write(traceback.format_exc())
            return str(e)
        
        return "OK"
    
    def clean(self, name):
        '''
        Clean up the files created bu the run method.
        '''
        os.remove(name+".stderr.temp")
        os.remove(name+".stdout.temp")
        
class Compare:
    '''
    Compare the test output to the validated output
    '''

    def __init__(self):
        self.msg = Message()

    def run(self, name):
        '''
        Compare the files with the given name using the system diff utiltity.
        If the two files are the same, then return zero, else if they are 
        different then return a non-zero number.

        Input name is like 'test.stdout'

        Returns:
        0 = files compare properly
        1 = files are different
        2 = error such as files not found
        '''
        try :
            # call the diff util
            oname = name+'.stdout.diff'
            ename = name+'.stderr.diff'
            ofh = open(oname, 'w')
            efh = open(ename, 'w')
            pr = subprocess.run(['diff', '-wBa', name+".temp", name], text=True, stdout=ofh, stderr=efh)
            ofh.close()
            efh.close()
            return pr.returncode
            
        except Exception as e:
            with open(os.path.join(dir, name+".error"), "w") as fp :
                fp.write(traceback.format_exc())
            return -1
        
    def clean(self, name):
        '''
        Clean up the files created by the comapre method.
        '''
        os.remove(name+".stdout.diff")
        os.remove(name+".stderr.diff")

class TestSpec :

    def __init__(self, level=10): 
        '''
        Open the input file and read it as a test spec.
        '''
        self.num_run = 0
        self.num_pass = 0
        self.num_skip = 0
        self.num_error = 0
        self.num_fail = 0
        self.num_tests = 0
        self.msg = Message()
        self.compiler = Compiler()
        self.compare = Compare()
        self.spec = self.read_spec(os.path.realpath(os.getcwd()), 'run')

    def report(self):
        self.msg.write(1, "\n\nREPORT: tests: %d, run: %d, skip: %d, pass: %d, fail: %d, errors: %d\n\n"%(
            self.num_tests, self.num_run, self.num_skip, self.num_pass, self.num_fail, self.num_error))

    def read_spec(self, dirname, action) :
        '''
        Recursively read the spec files and store them in a dict such that 
        skipped tests are propigated in the tree.
        '''
        with open(os.path.join(dirname, "test_list.txt"), "r") as fp:
            lines = fp.readlines()

        spec = {}
        spec['dirname'] = dirname
        for num, line in enumerate(lines):
            s = line.split("#")
            s = s[0].strip()
            if len(s) <= 0:
                continue

            x = s.split(":")
            if x[0] == "verbosity":
                if x[1].isdigit():
                    spec['verbosity'] = int(x[1])
                    if spec['verbosity'] > 10 or spec['verbosity'] < 0:
                        self.msg.write(0, "ERROR: %s: %d: verbosity value must between 0 and 10\n"%(
                            os.path.join(dirname, "test_list.txt"), num+1))
                        exit(1)
                else:
                    self.msg.write(0, "ERROR: %s: %d: verbosity must be a number\n"%(
                        os.path.join(dirname, "test_list.txt"), num+1))
                    exit(1)
            else:
                if x[1].lower() != 'skip' and x[1].lower() != 'run':
                    self.msg.write(0, "ERROR: %s: %d: valid test name values are 'run' and 'skip'\n"%(
                        os.path.join(dirname, "test_list.txt"), num+1))
                    exit(1)

                spec[x[0]] = {}
                newdir = os.path.join(dirname, x[0])
                if action.lower() == 'run' and x[1].lower() == 'run':
                    if os.path.isdir(newdir):
                        spec[x[0]]['type'] = 'dir'
                        spec[x[0]]['tests'] = self.read_spec(newdir, 'run')
                        spec[x[0]]['action'] = 'run'
                    else:
                        spec[x[0]]['type'] = 'file'
                        spec[x[0]]['action'] = 'run'
                elif action.lower() == 'skip' or x[1].lower() == 'skip':
                    if os.path.isdir(newdir):
                        spec[x[0]]['type'] = 'dir'
                        spec[x[0]]['tests'] = self.read_spec(newdir, 'skip')
                        spec[x[0]]['action'] = 'skip'
                    else:
                        spec[x[0]]['type'] = 'file'
                        spec[x[0]]['action'] = 'skip'

        return spec

    def get_spec(self):
        '''
        Return the test spec that was read form the disk.
        '''
        return self.spec
    
    def run(self): 
        '''
        Run the tests that are given in the spec
        '''
        self.run_dir(self.spec)

    def run_dir(self, spec):
        '''
        Recurse a directory where tests may be found.
        '''
        verbo = self.msg.peek()
        for item in spec:
            if item == 'dirname':
                dname = spec['dirname']
            elif item == 'verbosity':
                verbo = spec['verbosity']
            else:
                self.msg.push(verbo)

                if spec[item]['type'] == 'file':
                    self.run_test(item, dname, spec[item])
                elif spec[item]['type'] == 'dir':
                    if self.msg.get_count() == 0:
                        self.msg.write(8, "\n")

                    if self.msg.peek() > 8:
                        self.msg.write(8, "GROUP: %s\n"%(os.path.join(dname, item)))
                    else:
                        self.msg.write(8, "GROUP: %s\n"%(item))

                    self.msg.add_count()
                    self.run_dir(spec[item]['tests'])
                    self.msg.sub_count()

                self.msg.pop()

    def run_test(self, name, dirname, spec):
        '''
        Filter tests to be run or skipped and update the statistics.
        '''
        self.num_tests += 1
        if spec['action'] == 'run':
            self.msg.write(8, "TEST: %s: RUN"%(name))
            self.num_run += 1
            self.runner(name, dirname)
        else :
            self.msg.write(8, "TEST: %s: SKIP\n"%(name))
            self.num_skip += 1

    def runner(self, name, dirname):
        '''
        Actually perform the actions to run a test.
        '''
        flag = 'PASS'
        pname = os.path.join(dirname, name)
        s = self.compiler.run(pname)
        if 'OK' == s:
            n = pname+".stdout"
            v = self.compare.run(n)
            if v == 0:
                self.compare.clean(n)
            elif v == 1:
                flag = 'FAIL'
            elif v > 1 or v < 0:
                flag = None
                self.msg.write(8, " ERROR compare %s\n"%(n))
                self.num_error += 1

            n = pname+".stderr"
            v = self.compare.run(n)
            if v == 0:
                self.compare.clean(n)
            elif v == 1:
                flag = 'FAIL'
            elif v > 1 or v < 0:
                flag = None
                self.msg.write(8, " ERROR compare %s\n"%(n))
                self.num_error += 1

            if flag == 'PASS':
                self.compiler.clean(pname)
                self.msg.write(8, " PASS\n")
                self.num_pass += 1
            elif flag == 'FAIL':
                self.msg.write(8, " FAIL\n")
                self.num_fail += 1

        else:
            self.msg.write(8, " ERROR (%s)\n"%(s))
            self.num_error += 1

if __name__ == "__main__" :

    spec = TestSpec()
    spec.run()
    spec.report()
    #prt(spec.get_spec())

