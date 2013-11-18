
# ==================================
# Signal Sources
# ==================================


from pydlcs import *


# ===========================================
# Clock: skips 'skip' cycles in input clock
# ===========================================

class Clock (LG):
	# skips positive cycls = skip
	def __init__(self, name, skip=0):
		LG.__init__(self,name)
		self.skip = skip
		self.clk_out = Connector(self,'clk_out')
		self.clk_in = Connector(self,'clk_in', activates = 1)
		self.count = 0

	def evaluate (self):
		global DEBUG_SIMU
		if(self.clk_in.value):
			if(self.count != 0):
				self.clk_out.set(0)
				if(DEBUG_SIMU):
					print "Clock: %s => skipping clock" %(self.name)
				self.count = self.count - 1
			else:
				if (DEBUG_SIMU):
					print "Clock: %s => passing clock" %(self.name)
				self.clk_out.set(1)
				self.count = self.skip
		else:
			self.clk_out.set(0)
		

				

