#!/usr/bin/env python3
from collections import Counter
import sys
if __name__ == '__main__':
    c = Counter()
    for line in sys.stdin :
        line = line.rstrip()
        if '\t' in line:
            k, v = line.split('\t')
            c.update({k:int(v)})
        else :
            c.update({line : 1})
    for k, v in c.most_common():
        print(k, v, sep = '\t')
