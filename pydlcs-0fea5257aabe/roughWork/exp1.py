from gates import *
from gates import *
Simu  = SIMU('SIMU',start = 0)
Os = Ostream('OUT',stream =1)
Is = Istream(name = 'IN',fname = 'inp', stream =1)
n = Not('N1')
Simu.clk_out.connect([Os.clk_in,Is.clk_in])
Is.data_out.connect([n.A])
n.B.connect([Os.data_in])
Simu.start = 1
n.B.monitor = 1
