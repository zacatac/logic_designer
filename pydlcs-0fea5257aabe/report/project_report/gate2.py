# ===================================
# CLASS : GATE2
#		(2 INPUTS, 1 OUTPUT GATES)
#====================================		
class Gate2 (LG) :         # two input gates. Inputs A and B. Output C.
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A = Connector(self,'A', activates=1)
        self.B = Connector(self,'B', activates=1)
        self.C = Connector(self,'C')
