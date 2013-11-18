import matplotlib.pyplot as plt
import numpy as np
 
fig = plt.figure(facecolor='white')
x = np.linspace(0,2*np.pi,100)
y = 2*np.sin(x)
 
ax = fig.add_subplot(1,1,1, aspect='equal')
ax.set_title(r'$y=\sin(t)$')
ax.plot(x,y)
#ax.spines['left'].set_position('zero')
#ax.spines['right'].set_color('none')
#ax.spines['bottom'].set_position('zero')
#ax.spines['top'].set_color('none')
#ax.xaxis.set_ticks_position('bottom')
#ax.yaxis.set_ticks_position('left')
#ax.autoscale_view(tight=True)
ax.set_ylim(-2.5,2.5)
ax.set_xlim((0,2*np.pi))
ax.set_xticks([0,np.pi,2*np.pi])
ax.set_xticklabels(['', r'$\pi$', r'$2\pi$'])
ax.text(2*np.pi + .1, -.2, r'$t$') # Manually adjusted
 
plt.savefig('plot.pdf')
plt.show()
