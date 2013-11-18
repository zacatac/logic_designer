# ===================================
# CLASS : GATE2 (2 INPUTS, 1 OUTPUT GATES)
#====================================		
class Gate2 (LG) :         # two input gates. Inputs A and B. Output C.
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A = Connector(self,'A', activates=1)
        self.B = Connector(self,'B', activates=1)
        self.C = Connector(self,'C')

# ============================
# CLASS	:	AND GATE (And)
# ============================
class And (Gate2) :       # two input AND Gate
    def __init__ (self, name) :
        Gate2.__init__ (self, name)
    def evaluate (self) : self.C.set(self.A.value and self.B.value)
