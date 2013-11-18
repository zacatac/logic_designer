# ===================================
# CLASS :	HALF ADDER (HalfAdder)
# ===================================
class HalfAdder (LG) :         # One bit adder, A,B in. Sum and Carry out
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A = Connector(self,'A',1)
        self.B = Connector(self,'B',1)
        self.S = Connector(self,'S')
        self.C = Connector(self,'C')
        self.X1= Xor("X1")
        self.A1= And("A1")
        self.A.connect    ([ self.X1.A, self.A1.A])
        self.B.connect    ([ self.X1.B, self.A1.B])
        self.X1.C.connect ([ self.S])
        self.A1.C.connect ([ self.C])
