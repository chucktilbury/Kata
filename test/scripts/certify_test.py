#!/usr/bin/env python3

import os, sys

if len(sys.argv) < 2:
    print('need a file name\n')
    exit(1)

for item in sys.argv[1:] :
    
    # strip the ext if it exists
    name = os.path.splitext(item)[0]
    print("cert:", name)

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

    if os.path.isfile(name+".stdout.stderr.diff"):
        os.remove(name+".stdout.stderr.diff")

    if os.path.isfile(name+".stdout.stdout.diff"):
        os.remove(name+".stdout.stdout.diff")

    if os.path.isfile(name+".stderr.stderr.diff"):
        os.remove(name+".stderr.stderr.diff")

    if os.path.isfile(name+".stderr.stdout.diff"):
        os.remove(name+".stderr.stdout.diff")

if os.path.isfile("report.txt"):
    os.remove("report.txt")
