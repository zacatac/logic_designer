# CIRCUIT:
#
#	I_STREAM--->NOT_GATE---->AND.A
#		|
#		|							---->O_STREAM
#		|
#		-------------------->AND.B         
#
#	1. plotting working

from pydlcs import *
sim = SIMU('sim1',start = 0,plots = 1, debug =0 , step =0, pannotate = 1)

I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)

A1 = And('A1')
N = Not('N')

sim.clk_out.connect([I.clk_in, O.clk_in])
I.data_out.connect([A1.A, N.A])
N.B.connect([A1.B])
A1.C.connect([O.data_in])

sim.addplot([I.data,I.data,O.data])
sim.addpname(["input1", "input2","output"])

#sim.addplot([I.data])
#sim.addpname(["input"])


sim.start = 1
sim.simulate()

