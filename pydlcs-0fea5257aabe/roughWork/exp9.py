# CIRCUIT:
#

from pydlcs import *
sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk=1 , step =0, pannotate = 1)

I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)

CO = ConstSrc('CS',value =1)
A1 = And('A1')

sim.clk_out.connect([I.clk_in,CO.clk_in, O.clk_in])
I.data_out.connect([A1.A])
CO.data_out.connect([A1.B])
A1.C.connect([O.data_in])

sim.addplot([I.data,CO.data,O.data])
sim.addpname(["input1","const","output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

