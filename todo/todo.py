#!/usr/bin/python3
import argparse
import sys
import re
import os
import datetime


import json

home=os.environ['HOME']
rc=os.path.join(os.environ['HOME'],'.todoconfig')
conf=json.load(open(rc))
todotxt=conf['todo.txt'].replace('~',home)
todoarchive=(conf['todo.txt.archive']).replace('~',home)

print(todotxt)

class Color:
    RED=31
    GREEN=32
    YELLOW=33
    BLUE=34
    PURPLE=35


def make_color(s,color=36):
    color=str(color)
    return '\033['+color+';01m%s\033[1;m'%(str(s)) #blue



class Record :
    def date_color(self):
        if self.finishdate :
            return Color.BLUE
        if not self.duedate :
            return Color.PURPLE
        today=datetime.date.today()
        deltaday=(self.duedate-today).days
        if deltaday<0 : return Color.RED
        if deltaday==0 : return Color.YELLOW
        if deltaday>0 : return Color.GREEN
    @staticmethod
    def expend_date(date):
        today=datetime.date.today()
        deltaday=(date-today).days
        hint=('today' if deltaday==0 else
                str(deltaday)+'天后' if deltaday>0 else
                str(-deltaday)+'天前'
                )
        return hint+'('+str(date)+')'
    
    def byon(self,match):
        #date
        spec=match.group('spec')
        if spec :
            if spec=='today' :
                delta=datetime.timedelta(days=0)
            elif spec=='tomorrow' :
                delta=datetime.timedelta(days=1)
            elif spec=='yesterday' :
                delta=datetime.timedelta(days=-1)
            date=datetime.date.today()+delta
        else :
            date=list(map(int,(match.group('year'),match.group('mon'),match.group('day'))))
            if date[0]<1000 : date[0]+=2000
            date=datetime.date(*date)
        byon=match.group('byon')
        if byon=='on' :
            self.finishdate=date
        if byon=='by' :
            self.duedate=date
        return ''
    def update(self,line) :
        line=line.strip()
        line=re.sub(r'''(?P<byon>by|on)\ ((?P<spec>today|yesterday|tomorrow)|(?P<year>[0-9]{2,4})-(?P<mon>[0-9]{1,2})-(?P<day>[0-9]{1,2}))''',self.byon,line)
        line=line.strip()
        if line :
            self.content=line
    def __init__(self,ln,line):
        self.line_number=ln
        self.duedate=None
        self.finishdate=None
        self.update(line)
    def __str__(self):
        color=self.date_color()
        duedate=(
                make_color('需要完成于'+self.expend_date(self.duedate),color)
                if self.duedate else '')
        finishdate=(
                make_color('已经完成于'+self.expend_date(self.finishdate),color)
                if self.finishdate else '')
        line= self.content+' '+duedate+' '+finishdate
        ind=make_color("%5s"%str(self.line_number))+': '
        return ind+line
    def key(self):
        return (
                self.finishdate is None,
                self.duedate if self.duedate else datetime.date(3000,1,1)
                )
    def dumps(self):
        dd='' if not self.duedate else 'by '+str(self.duedate)
        fd='' if not self.finishdate else 'on '+str(self.finishdate)
        return self.content+' '+dd+' '+fd

class Todo :
    def parse(self,ln,line):
        return (duedate,ln,line)

    def load(self):
        data=[]
        for ln,line in enumerate(open(self.filename)) :
            record=Record(ln+1,line)
            data.append(record)
        return data
    def save(self,data):
        file=open(self.filename,"w")
        for x in data:
            print(x.dumps(),file=file)
        file.close()
        

    def ls(self,argv):
        data=[]
        for ln,line in enumerate(open(self.filename)) :
            record=Record(ln+1,line)
            data.append(record)
        data=sorted(data,key=lambda x:x.key())
        for item in data:
            print(item)
    def do(self,argv):
        line=' '.join(argv)
        record=Record(0,line)

        file=open(self.filename,'a')
        print(record.dumps(),file=file)
        file.close()
    def delete(self,argv):
        delln=int(argv[0])
        lines=[]
        for ln,line in enumerate(open(self.filename)):
            ln+=1
            if ln!=delln:
                lines.append(line.strip())

        file=open(self.filename,'w')
        for line in lines:
            print(line,file=file)
        file.close()

    def check(self,argv):
        ind=int(argv[0])-1
        data=self.load()
        data[ind].finishdate=datetime.date.today()
        self.save(data)

    def update(self,argv):
        ind=int(argv[0])-1
        line=' '.join(argv[1:])
        data=self.load()
        data[ind].update(line)
        self.save(data)

    def uncheck(self,argv):
        ind=int(argv[0])-1
        data=self.load()
        data[ind].finishdate=None
        self.save(data)
    def archive(self,argv):
        data=self.load()
        rest=[d for d in data if d.finishdate is None]
        data=[d for d in data if d.finishdate is not None]
        self.save(rest)
        file=open(self.filename+'.archive',"a")
        for d in data :
            print(d.dumps(),file=file)



    def __init__(self,filename=os.path.join(sys.path[0],todotxt)):
        self.functions={
                'ls': self.ls,
                'do': self.do,
                'check': self.check,
                'uncheck': self.uncheck,
                'update': self.update,
                'add': self.do,
                'del': self.delete,
                'archive': self.archive,
                }
        self.filename=filename
    
if __name__ == '__main__':
    todo=Todo()
    #parser=argparse.ArgumentParser(description="")
    #parser.add_argument('command',help='command',default='ls')
    #args=parser.parse_args()
    if len(sys.argv)==1 :
        sys.argv.append('ls')
    if sys.argv[1] in todo.functions :
        todo.functions[sys.argv[1]](sys.argv[2:])


