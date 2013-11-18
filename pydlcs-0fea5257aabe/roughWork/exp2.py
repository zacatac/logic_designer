from gates import *

Simu  = SIMU('SIMU',start = 0)
Os = Ostream('OUT',stream =1)
Is = Istream(name = 'IN',fname = 'inp', stream =1)
print Is.data
Simu.clk_out.connect([Is.clk_in,Os.clk_in])
Is.data_out.connect([Os.data_in])
Simu.start = 1
Simu.simulate()
