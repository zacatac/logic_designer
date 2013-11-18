# ==========================
# CLASS	:	NOT GATE (Not)
# ==========================
class Not (LG) :         # Inverter. Input A. Output B.
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A = Connector(self,'A', activates=1)
        self.B = Connector(self,'B')
    def evaluate (self) : self.B.set(not self.A.value)
