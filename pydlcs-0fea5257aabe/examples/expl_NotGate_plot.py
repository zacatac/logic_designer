# CIRCUIT:
#
#	I_STREAM--->NOT_GATE---->O_STREAM
#
#	1. plotting working

from pydlcs import *
from pylab import *
sim = SIMU('sim1',start = 0,plots = 1, debug =1 ,pannotate = 1, step =0)

I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)
N = Not('N')


sim.clk_out.connect([I.clk_in, O.clk_in])
I.data_out.connect([N.A])
N.B.connect([O.data_in])

sim.addplot([I.data,O.data])
sim.addpname(["input","output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

