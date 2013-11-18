# CIRCUIT:
#
#	I_STREAM--->NOT_GATE---->O_STREAM
#


from gates import *

sim = SIMU('sim1',start = 0, debug =1 , step =0)

I = Istream('IN', fname = 'inp', stream =1)
O = Ostream('OUT', stream = 1)
N = Not('N')

sim.clk_out.connect([I.clk_in, O.clk_in])
I.data_out.connect([N.A])
N.B.connect([O.data_in])

sim.start = 1
sim.simulate()
O.data
