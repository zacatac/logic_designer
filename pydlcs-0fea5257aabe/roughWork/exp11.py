# CIRCUIT:
#

from sequence import *

sim = SIMU('sim1',start = 0,plots = 1, debug =1, pclk = 1 , step =0, clocks =20, pannotate=1)

#I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)

FD_TF = FD_TFlipFlop('FD_TF')

sim.clk_out.connect([FD_TF.C, O.clk_in])
#FD_DF.Qbar.connect([FD_DF.D])
FD_TF.Q.connect([O.data_in])

sim.addplot([O.data])
sim.addpname(["output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

