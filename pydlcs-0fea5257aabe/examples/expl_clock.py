# clock - checking clock source
#

from pydlcs import *


sim = SIMU('simu', plots =1, debug =1, pclk = 1, pannotate =1)

C = Clock('C', skip = 2)
I = Istream('I', fname = 'inp', stream = 1)
O = Ostream ('O', stream = 1)

sim.clk_out.connect([I.clk_in, C.clk_in,  O.clk_in])
C.clk_out.connect([O.data_in])

sim.addplot([O.data])
sim.addpname(["out"])

sim.start = 1
sim.simulate()



