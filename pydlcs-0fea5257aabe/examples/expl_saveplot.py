# CIRCUIT:
#

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0, saveplots = "save.pdf" )

I1 = Istream('IN',fname = 'inp', stream =1)
#I2 = Istream('IN', fname = 'inp2', stream =1)
O = Ostream('OUT', stream = 1)

JK = JKFlipFlop('JK')

#sim.clk_out.connect([I1.clk_in, I2.clk_in, JK.C, O.clk_in])
sim.clk_out.connect([I1.clk_in, JK.C, O.clk_in])
I1.data_out.connect([JK.J])
#I2.data_out.connect([JK.K])
I1.data_out.connect([JK.K])
JK.Q.connect([O.data_in])

sim.addplot([I1.data,I1.data,O.data])
sim.addpname(["input_J","input_K","output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

