
class NAND3 (LG):
 def __init__(self, name):
                LG.__init__(self,name)
                self.A = Connector(self,'A')
                self.B = Connector(self,'B')
                self.C = Connector(self,'C')
                self.D = Connector(self,'D', activates = 1)
                

 def evaluate (self):
               self.D.set(self.A.value and self.B.value and self.C.value)

