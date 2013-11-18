# =========================
# CLASS	:	OR GATE (Or)
#==========================
class Or (Gate2) :         # two input OR gate.
    def __init__ (self, name) :
        Gate2.__init__ (self, name)
    def evaluate (self) : self.C.set(self.A.value or self.B.value)

