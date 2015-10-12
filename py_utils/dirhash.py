#!/usr/bin/env python3
import sys
import os
import random


class FileBuffer:
    def __init__(self, f):
        self.f = f
        self.buf = []
    def __call__(self, l):
        self.buf.append(l)
        if len(self.buf) == 10000 :
            for b in self.buf :
                print(b, file = self.f)
            self.buf = []
            
    def __del__(self):
        for b in self.buf :
            print(b, file = self.f)

if __name__ == '__main__':
    tgt = sys.argv[1]
    n = 100
    os.system('mkdir %s -p' % tgt)

    files = [open(os.path.join(tgt, "%d" % i), 'w') for i in range(n)]
    files = [FileBuffer(f) for f in files]

    for i, line in enumerate(sys.stdin) :
        if i % 10000 == 0:
            print(i / 10000, file =sys.stdout, end = '\r')
        line = line.rstrip()#.split()
        #if len(line) != 2 : continue
        #k, v = line
        k = line.partition('\t')[0]
        i = hash(k) % n
        files[i](line)
        #print(k, v, sep = '\t', file = files[i])

