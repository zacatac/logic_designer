# =================================
# CLASS : FULL ADDER (FullAdder)
# =================================

class FullAdder (LG) :         # One bit adder, A,B,Cin in. Sum and Cout out
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A    = Connector(self,'A',activates = 1)
        self.B    = Connector(self,'B',activates = 1)
        self.Cin  = Connector(self,'Cin',activates = 1)
        self.S    = Connector(self,'S')
        self.Cout = Connector(self,'Cout')
        self.H1= HalfAdder("H1")
        self.H2= HalfAdder("H2")
        self.O1= Or("O1")
        self.A.connect    ([ self.H1.A ])
        self.B.connect    ([ self.H1.B ])
        self.Cin.connect  ([ self.H2.A ])
        self.H1.S.connect ([ self.H2.B ])
        self.H1.C.connect ([ self.O1.B])
        self.H2.C.connect ([ self.O1.A])
        self.H2.S.connect ([ self.S])
        self.O1.C.connect ([ self.Cout])
