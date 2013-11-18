# clock - checking clock source
# MUX example

from pydlcs import *


sim = SIMU('simu', plots =1, debug =1, pclk = 1, pannotate =1,)

I1 = Istream('I1', fname = 'inp', stream = 1)
I0 = Istream('I0', fname = 'inp2', stream = 1)
O = Ostream ('O',fname = "opt", stream = 1)

M = Mux('M')

sim.clk_out.connect([I1.clk_in, I0.clk_in, M.Sel,  O.clk_in])
I1.data_out.connect([M.D1])
I0.data_out.connect([M.D0])

M.O.connect([O.data_in])


sim.addplot([I1.data, I0.data, O.data])
sim.addpname(["I1", "I0","out"])

sim.start = 1
sim.simulate()



