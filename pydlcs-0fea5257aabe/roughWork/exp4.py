from gates import *
I = Istream('I',fname = 'inp', stream = 1)
O = Ostream('O',stream = 1)
I.data_out.connect([O.data_in])
N = Not('N')
N.B.connect([I.clk_in, O.clk_in])
N.A.set(1)
N.A.set(0)
N.A.set(1)
N.A.set(0)
print DEBUG_SIMU
DEBUG_SIMU = 1
print DEBUG_SIMU
N.A.set(1)
N.A.set(0)
