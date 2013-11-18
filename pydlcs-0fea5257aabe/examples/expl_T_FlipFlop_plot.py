# CIRCUIT:
#
#	I_STREAM--->T Filp Flop---->O_STREAM
#
#	1. hi
#	1. plotting working

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0, pannotate = 1)

I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)

TF = TFlipFlop('TF')

sim.clk_out.connect([I.clk_in, TF.C, O.clk_in])
I.data_out.connect([TF.T])
TF.Q.connect([O.data_in])

sim.addplot([I.data,O.data])
sim.addpname(["input","output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

