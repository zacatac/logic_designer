# CIRCUIT:
# 4 Bit Serial In Parallel Out Shift Register

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0)

I = Istream('IN', fname = 'inp4', stream =1)

O1 = Ostream('OUT', stream = 1)
O2 = Ostream('OUT', stream = 1)
O3 = Ostream('OUT', stream = 1)
O4 = Ostream('OUT', stream = 1)

D1 = DFlipFlop('D1')
D2 = DFlipFlop('D2')
D3 = DFlipFlop('D3')
D4 = DFlipFlop('D4')

sim.clk_out.connect([I.clk_in, D4.C, O4.clk_in, D3.C, O3.clk_in, D2.C, O2.clk_in, D1.C, O1.clk_in])
I.data_out.connect([D1.D])
D1.Q.connect([D2.D, O1.data_in])
D2.Q.connect([D3.D, O2.data_in])
D3.Q.connect([D4.D, O3.data_in])
D4.Q.connect([O4.data_in])

sim.addplot([O1.data, O2.data, O3.data, O4.data])
sim.addpname(["Q1", "Q2", "Q3", "Q4"])

#sim.addplot([O.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

