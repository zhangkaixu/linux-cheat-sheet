#!/usr/bin/env python3
import sys
import os
import random

if __name__ == '__main__':
    tgt = sys.argv[1]
    n = 100
    os.system('mkdir %s -p' % tgt)

    keys = {}
    files = [open(os.path.join(tgt, "%d" % i), 'w') for i in range(n)]

    for line in sys.stdin :
        line = line.rstrip().split()
        if len(line) != 2 : continue
        k, v = line
        if k not in keys :
            i = random.randint(0, n - 1)
            keys[k] = files[i]
        print(k, v, sep = '\t', file = keys[k])

