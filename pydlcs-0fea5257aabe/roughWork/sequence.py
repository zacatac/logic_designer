
# ============================================
# Sequential Circuits
# ============================================


from pydlcs import *

# ================
# D - FlipFlop
# ================

class DFlipFlop (LG):

	def __init__(self, name):
		LG.__init__(self,name)
		self.D = Connector(self,'D')
		self.Q = Connector(self,'Q')
		self.Qbar = Connector(self,'Qbar')
		self.C = Connector(self,'C', activates = 1)
		self.Q.value = 0
		self.prev = 0
		self.Qbar.value = not self.Q.value
		
	def evaluate (self):
		if (not self.C.value) and self.prev:	# clock drop
			self.Q.set(self.D.value)
		self.prev = self.C.value
		self.Qbar.set(not self.Q.value)

		
		
# ================
# T - FlipFlop
# ================

class TFlipFlop (LG):

	def __init__(self, name):
		LG.__init__(self,name)
		self.T = Connector(self,'T')
		self.Q = Connector(self,'Q')
		self.C = Connector(self,'C', activates = 1)
		self.Q.value = 0
		self.prev = 0
		
	def evaluate (self):
		if (not self.C.value) and self.prev and self.T.value:	# clock drop and T=1
			self.Q.set(not self.Q.value)
		self.prev = self.C.value


# ================
# JK - FlipFlop
# ================

class JKFlipFlop (LG):

	def __init__(self, name):
		LG.__init__(self,name)
		self.J = Connector(self,'J')
		self.K = Connector(self,'K')
		self.Q = Connector(self,'Q')
		self.C = Connector(self,'C', activates = 1)
		self.Q.value = 0
		self.prev = 0
		
	def evaluate (self):
		if (not self.C.value) and self.prev and self.J.value and (not self.K.value):	
			self.Q.set(1)
		if (not self.C.value) and self.prev and not(self.J.value) and self.K.value:	
			self.Q.set(0)
		if (not self.C.value) and self.prev and self.J.value and self.K.value:	
			self.Q.set(not self.Q.value)
		self.prev = self.C.value


# ==========================================
# Frequencu Division by using T - FlipFlop
# ==========================================

class FD_TFlipFlop (LG):

	def __init__(self, name):
		LG.__init__(self,name)
		self.T = Connector(self,'T')
		self.Q = Connector(self,'Q')
		self.C = Connector(self,'C', activates = 1)
		self.Q.value = 0
		self.prev = 0
		#self.T.value
		
	def evaluate (self):
		if (not self.C.value) and self.prev: 
			self.Q.set(not self.Q.value)
		self.prev = self.C.value


