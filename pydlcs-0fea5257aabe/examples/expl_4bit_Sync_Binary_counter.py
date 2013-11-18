# CIRCUIT:
# A 4 Bit Syncronous Binary Counter using JK Flip Flpos 

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0, clocks=33)

CO = ConstSrc('CS',value =1)

O1 = Ostream('OUT1', stream = 1)
O2 = Ostream('OUT2', stream = 1)
O3 = Ostream('OUT3', stream = 1)
O4 = Ostream('OUT4', stream = 1)

JK1 = JKFlipFlop('JK1')
JK2 = JKFlipFlop('JK2')
JK3 = JKFlipFlop('JK3')
JK4 = JKFlipFlop('JK4')

A1 = And('A1')
A2 = And('A2')


sim.clk_out.connect([JK4.C, O4.clk_in, JK3.C, O3.clk_in, JK2.C, O2.clk_in,CO.clk_in, JK1.C, O1.clk_in])
CO.data_out.connect([JK1.J, JK1.K])
JK1.Q.connect([JK2.J, JK2.K, A1.A, O1.data_in])
JK2.Q.connect([A1.B, O2.data_in])
A1.C.connect([JK3.J, JK3.K, A2.A])
JK3.Q.connect([A2.B, O3.data_in])
A2.C.connect([JK4.J, JK4.K])
JK4.Q.connect([O4.data_in])

sim.addplot([O1.data, O2.data, O3.data, O4.data])
sim.addpname(["Q1", "Q2", "Q3", "Q4"])

sim.start = 1
sim.simulate()

