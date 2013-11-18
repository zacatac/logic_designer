class SR_NOR_LATCH_C (LC):
              def __init__ (self, name) :
                LC.__init__ (self, name)
                self.SRL= SR_LATCH('SRL')
                self.S = Connector(self,'A',1)
                self.R = Connector(self,'B',1)
                self.Q = Connector(self,'Q',monitor=1)
                self.Qb = Connector (self,'Qb',monitor=1)
                self.E = connector (self,'E',1)
                self.A1= And('A1')
                self.A2= And ('A2')
                self.E.connect ([self.A1.B,self.A2.A])
                self.S.connect ([self.A1.A])
                self.R.connect ([self.A2.B])
                self.SRL.S.connect ([self.A1.C])
                self.SRL.R.connect ([self.A2.c])
                self.Q.connect ([self.SRL.Q])
                self.Qb.connect ([self.SRL.Qb])

