# CIRCUIT:
#

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0)

I1 = Istream('IN', fname = 'inp3', stream =1)

O1 = Ostream('OUT', stream = 1)
O2 = Ostream('OUT', stream = 1)
O3 = Ostream('OUT', stream = 1)
O4 = Ostream('OUT', stream = 1)

JK1 = JKFlipFlop('JK1')
JK2 = JKFlipFlop('JK2')
JK3 = JKFlipFlop('JK3')
JK4 = JKFlipFlop('JK4')

sim.clk_out.connect([I1.clk_in, JK1.C, O1.clk_in, O2.clk_in, O3.clk_in, O4.clk_in])
I1.data_out.connect([JK1.J])
I1.data_out.connect([JK1.K])
I1.data_out.connect([JK2.J])
I1.data_out.connect([JK2.K])
I1.data_out.connect([JK3.J])
I1.data_out.connect([JK3.K])
I1.data_out.connect([JK4.J])
I1.data_out.connect([JK4.K])


JK1.Q.connect([JK2.C, O1.data_in])
JK2.Q.connect([JK3.C, O2.data_in])
JK3.Q.connect([JK4.C, O3.data_in])
JK4.Q.connect([O4.data_in])

sim.addplot([O1.data, O2.data, O3.data, O4.data])
sim.addpname(["Q1", "Q2", "Q3", "Q4"])

#sim.addplot([O.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

