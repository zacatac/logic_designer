
from pylab import *

def plott(plt):
	l = len(plt)
	i=1;
	for p in plt:
		subplot(l,1,i)
		num = range(len(p));
		step(num,p)
		ylim(-0.5,1.5)
		i=i+1
		
		
a = [1,0,1,0,0,0,1,1,0,1]
b = [1,0,1,0,0,0,1,1,0,1]
c = [1,0,1,0,0,0,1,1,0,1]

lst=[]
lst.append(a)
lst.append(b)
lst.append(c)

plott(lst)
show()
