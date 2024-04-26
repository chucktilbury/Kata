#!/usr/bin/env python3

import os, sys

if len(sys.argv) < 2:
    print('need a file name\n')
    exit(1)

for name in sys.argv[1:] :
    if os.path.isfile(name+".stdout.temp") :
        os.remove(name+".stdout")
        os.rename(name+".stdout.temp", name+".stdout")

    if os.path.isfile(name+".stderr.temp") :
        os.remove(name+".stderr")
        os.rename(name+".stderr.temp", name+".stderr")

    if os.path.isfile(name+".stdout.fail") :
        os.remove(name+".stdout.fail")

    if os.path.isfile(name+".stderr.fail") :
        os.remove(name+".stderr.fail")


