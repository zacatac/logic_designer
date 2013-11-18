# clock - checking clock source
#
#

from pydlcs import *


sim = SIMU('simu', plots =1, debug =1, pclk = 1, pannotate =1,)

I = Istream('I', fname = 'inp', stream = 1)
O = Ostream ('O',fname = "opt", stream = 1)
E = EdgeClock('E')

sim.clk_out.connect([O.clk_in, E.clk_in])
E.clk_out.connect([I.clk_in])

I.data_out.connect([O.data_in])

sim.addplot([I.data,O.data])
sim.addpname(["in","out"])

sim.start = 1
sim.simulate()



