#!/usr/bin/env python3
import sys
import os
import subprocess
import multiprocessing
import argparse

class Mapper :
    def __init__(self, cmd):
        self._cmd = cmd
    def __call__(self, args):
        src = args['src']
        tgt = args['des']
        print(src, file = sys.stderr)
        cmd = '%s | %s > %s'%(args['cat']%(src), self._cmd, tgt)
        print(cmd)
        os.system(cmd)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='对一个文件夹中的文件执行同样的映射')
    parser.add_argument('mapper', type=str, help='mapper') 
    parser.add_argument('src', type=str, help='源目录') 
    parser.add_argument('des', type=str, help='目标目录') 
    parser.add_argument('--cat', type=str, default='cat %s', help='') 
    parser.add_argument('-p', type=int, default=1, help='进程个数') 
    args = parser.parse_args()

    dirname = args.src
    tgtdir = args.des
    mapper = args.mapper


    if mapper :
        os.system('mkdir -p %s' % tgtdir)

        files = sorted(os.listdir(dirname))
        files = [{'src' :os.path.join(dirname, f), 
            'des' :os.path.join(tgtdir, f),
            'cat' : args.cat,
            } for f in files]
        pool = multiprocessing.Pool(args.p)
        pool.map(Mapper(mapper), files)

