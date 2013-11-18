
class SR_NAND_LATCH (LG):

 def __init__(self, name):
                LG.__init__(self,name)
                self.S = Connector(self,'S')
                self.R = Connector(self,'R')
                self.Q = Connector(self,'Q')
                self.C = Connector(self,'C', activates = 1)
                self.Q.value = 0
                self.prev = 0

 def evaluate (self):

                if (not self.C.value) and self.prev and self.S.value and (not self.R.value):
                        self.Q.set(0)
                if (not self.C.value) and self.prev and (not self.S.value) and self.R.value:
                        self.Q.set(1)
                if (not self.C.value) and self.prev and self.S.value and self.R.value:
                        self.Q.set( self.Q.value)
                if (not self.C.value) and self.prev and (not self.S.value) and (not self.R.value):
                       self.Q.set(0 )
                self.prev = self.C.value



class SR_NOR_LATCH (LG):
 def __init__(self, name):
                LG.__init__(self,name)
                self.S = Connector(self,'S')
                self.R = Connector(self,'R')
                self.Q = Connector(self,'Q')
                self.C = Connector(self,'C', activates = 1)
                self.Q.value = 0
                self.prev = 0

 def evaluate (self):

                if (not self.C.value) and self.prev and self.S.value and (not self.R.value):
                        self.Q.set(1)
                if (not self.C.value) and self.prev and (not self.S.value) and self.R.value:
                        self.Q.set(0)
                if (not self.C.value) and self.prev and self.S.value and self.R.value:
                        self.Q.set( self.Q.value)
                if (not self.C.value) and self.prev and (not self.S.value) and (not self.R.value):
                       self.Q.set(1 )
                self.prev = self.C.value



class JK_LATCH (LG):
 def __init__(self, name):
                LG.__init__(self,name)
                self.S = Connector(self,'J')
                self.R = Connector(self,'K')
                self.Q = Connector(self,'Q')
                self.C = Connector(self,'C', activates = 1)
                self.Q.value = 0
                self.prev = 0

 def evaluate (self):

                if (not self.C.value) and self.prev and self.J.value and (not self.R.value):
                        self.Q.set(1)
                if (not self.C.value) and self.prev and (not self.K.value) and self.R.value:
                        self.Q.set(0)
                if (not self.C.value) and self.prev and self.J.value and self.K.value:
                        self.Q.set( self.Q.value)
                if (not self.C.value) and self.prev and (not self.J.value) and (not self.K.value):
                       self.Q.set(not self.Q.value )
                self.prev = self.C.value

