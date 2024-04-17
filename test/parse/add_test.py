#!/usr/bin/env python3

import os, sys

if len(sys.argv) < 2:
    print('need a file name\n')
    exit(1)

for name in sys.argv[1:] :
    with open(name+".k", 'w') as fh :
        fh.write(';'+'-'*79+'\n;\n;'+'-'*79)

    with open(name+".stderr", 'w') as fh :
        pass

    with open(name+".stdout", 'w') as fh :
        pass

    os.system('echo "%s:run" >> test_list.txt'%(name))