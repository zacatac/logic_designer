# CIRCUIT: D Flip Flop
#

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0, pannotate = 1)

I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)

DF = DFlipFlop('DF')

sim.clk_out.connect([I.clk_in, DF.C, O.clk_in])
I.data_out.connect([DF.D])
DF.Q.connect([O.data_in])

sim.addplot([I.data,O.data])
sim.addpname(["input","output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

