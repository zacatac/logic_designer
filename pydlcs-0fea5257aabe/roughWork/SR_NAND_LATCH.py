class SR_NAND_LATCH (LC):
              def __init__ (self, name) :
                LC.__init__ (self, name)
                self.S = Connector(self,'A',1)
                self.R = Connector(self,'B',1)
                self.Q = Connector(self,'Q',monitor=1)
                self.Qb = Connector (self,'Qb',monitor=1)
                slef.N1 = Nand('N1')
                self.N2 = Nand('N2')
                self.S.connect ([self.N1.A])
                self.Qb.conect ([self.N1.B])
                self.Q.connect ([self.N1.C])
                self.R.connect ([self.N2.B])
                self.Q.conect ([self.N2.A])
                self.Qb.connect ([self.N2.C])

