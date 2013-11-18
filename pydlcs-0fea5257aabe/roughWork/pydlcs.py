



# ========================================================================
#  p y d l c s . p y
#
#  Logic Circuit Simulator 
#
#  Define classes for	Connector, 
#						Logic Gates ( AND, OR, NOT, XOR),
#						Combinational Circuits (HalfAdder, FullAdder, Mux/Demux)
#						Stream (input/output bit streams)
#						Plotting
#
# =======================================================================

# =======================
# LIBRARIES: 
# ======================

from pylab import *
from sys import *

# ===============
# GLOBALS :
# ===============

STOP_SIMU = 0	# STOP SIGNAL
DEBUG_SIMU = 0	# DEBUG OPTION ON/OFF
ERROR_SIMU = 0	# ANY ERROR OCCURED


# ====================
# CLASS	:	Connector
# ====================

class Connector :
    # Connectors are inputs and outputs. Only outputs should connect
    # to inputs. Be careful NOT to have circular references
    # As an output is changed it propagates the change to its connected inputs

    def __init__ (self, owner, name, activates=0, monitor=0) :
        self.value = 0
        self.owner = owner
        self.name  = name
        self.monitor  = monitor
        self.connects = []
        self.activates= activates   # If true change kicks evaluate function

    def connect (self, inputs) :
        if type(inputs) != type([]) : inputs = [inputs]
        for input in inputs : self.connects.append(input)


    def set (self, value) :
        if self.value == value : return      # Ignore if no change
        self.value = value
        if self.activates : self.owner.evaluate()
        if self.monitor :
			print "Connector: %s-%s set to %d" % (self.owner.name,self.name,self.value)	
        for con in self.connects : con.set(value)


#===================
# CLASS : STREAM
# ==================

class Ostream:

	#	pin - connected to o/p pin
	#	toggle clock to change o/p
	
	def __init__(self, name,stream = 0,fname = 0) :
		global DEBUG_SIMU
		self.name = name
		self.fname = fname
		self.data = []	# output data
		self.stream = stream
		self.clk_in = Connector(self,'clk_in', activates =1)
		self.data_in = Connector(self,'data_in')
		if DEBUG_SIMU:
			if self.fname:
				print "\nOstream: %s => output file name - %s" %(self.name, self.fname)
				self.fptr = open(fname,"w")


	def evaluate (self):
		global STOP_SIMU
		global DEBUG_SIMU
		if self.clk_in and self.stream and (not STOP_SIMU):
			self.data.append(int(self.data_in.value))
			if self.fname:
				self.fptr.write(str(int(self.data_in.value)))
			if STOP_SIMU:
				self.fptr.close()

			if(DEBUG_SIMU) :
				print "Ostream: %s => data = %d , buffer_data =" %(self.name, self.data_in.value)
				print ''.join([str(item) for item in self.data])
				print "\n\n"




class Istream:
	"""  This is Istream class 	"""

	def __init__ (self,name,fname = 0, stream = 0):
		global DEBUG_SIMU
		global ERROR_SIMU
		self.name = name
		self.fname = fname
		self.stream = stream
		self.clk_in = Connector(self,'clk_in', activates = 1)
		self.data_out = Connector(self,'data_out')
		if ( not fname):
			ERROR_SIMU = 1
			print "Istream: %s => ERROR - file name not given, fname?" %(self.name)
			sys.exit(1)
		else:
			self.data = self.ReadFile()
			self.data_curr = 0
			self.data_max = len(self.data)

		if DEBUG_SIMU:
			print "\nIstream: %s => input file name - %s" %(self.name, self.fname)


	def ReadFile(self):
		# read self.fname and convert to list ( with '\n' as last item
		f = open(self.fname)
		d = f.read()
		data = []
		tmp =[]
		for n in d:
			tmp.append(n)
		for i in range(len(tmp)-1):
			data.append(int(tmp[i]))
		return data	



	def evaluate (self):
		global STOP_SIMU
		if self.clk_in  and self.stream  and (self.data_curr < self.data_max ) and (self.data_max > 0) and (not STOP_SIMU):
			if (DEBUG_SIMU):
				print "Istream: %s => data = %d, data_curr = %d, data_max = %d" %(self.name, int(self.data[self.data_curr]), self.data_curr+1, self.data_max)
			self.data_out.set(int(self.data[self.data_curr]))
			self.data_curr = self.data_curr + 1
		else:
			if (DEBUG_SIMU):
				print "Istream: %s => Data stream exhausted...\n" %(self.name)
			STOP_SIMU = 1



# ==================
# CLASS : SIMULATOR
# ==================
#
# Description:
#		
#		Require ONE instance per simulator.
#

class SIMU :

	def __init__ (self,name, debug = 0, start = 0, step = 0, plots = 0, pclk = 0, pannotate = 0, clocks = 0, saveplots = 0):
		global DEBUG_SIMU
		global ERROR_SIMU
		global STOP_SIMU
		self.start = start
		self.step = step
		self.saveplots = saveplots
		self.clocks = clocks
		self.name = name
		self.debug = debug
		self.pannotate = pannotate
		self.plots = plots
		self.pclk = pclk
		self.plists = []
		self.pnames = []
		self.clk = []
		self.clk_out = Connector(self,'clk_out')
		DEBUG_SIMU = self.debug
		self.tclocks = self.clocks
		
	def simulate (self):
		if self.start :
			if (DEBUG_SIMU):
				print "\n\n\n"
				print "********************************************"
				print "* pydlcs - Digital Logic Circuit Simulator *"
				print "********************************************\n"
				print "_____________________________________________\n"
				print "            SIMULATION SUMMARY: %s" %(self.name)
				print "        ***NOTE: 0-DISABLED, 1-ENABLED***"
				print "plots - %d" %(self.plots)
				print "plot annotation - %d" %(self.pannotate)
				print "simulation clock cycles - %d" %(self.clocks)
				print "simulation start - %d" %(self.start)
				print "simulation debug option - %d" %(self.debug)
				print "simulation clock plot - %d" %(self.pclk)
				print "step execution - %d" %(self.step)
				print "______________________________________________\n"


				print "SIMULATOR: %s ==> Simulation Started...\n" %(self.name)
			while True :
				global STOP_SIMU
				global ERROR_SIMU

				if ERROR_SIMU:
					print "SIMULATOR: %s => Exiting due to previous errors..." %(self.name)

				if (STOP_SIMU):
					if (not ERROR_SIMU):
						if self.plots:
							if (not self.clocks):
								del self.clk[-1]	# simulation ended @ last clock
							self.PlotLists()	# plotting
					else:
						print "SIMULATOR: %s => EXITED WITH ERROR ..." %(self.name)

					if (DEBUG_SIMU):
						print "SIMULATOR: %s => Simulation Ended ...\n" %(self.name)
					break
				else:
					self.clk_out.set(not self.clk_out.value)

					if self.clocks:
						self.tclocks = self.tclocks - 1
						if (not self.tclocks):
							STOP_SIMU = 1
							if (DEBUG_SIMU):
								print "SIMULATOR : %s => clocks exhaused..."%(self.name)

					self.clk.append(self.clk_out.value)
					if (DEBUG_SIMU):
						print "SIMULATOR: %s => clk_out=%d" %(self.name, self.clk_out.value)
		else:
			print " SIMULATOR: %s => Please set start flag : %s.start=%d\n" %(self.name, self.name, self.start)
	

	def evaluate (self):
		pass

	def PlotLists(self):
		plts = len(self.plists)
		if plts == 0:
			print "ERROR: Nothing to Plot, please pass lists to plot %s.plists=<list-of-lists>\n" %(self.name)
		else:
			i=1
			for p in self.plists:
				if self.pclk:
					subplot(plts+1,1,i)
				else:
					subplot(plts,1,i)
				num = range(len(p))
				step(num,p)

				if self.pannotate:
					k=0
					for j in p:
						annotate("%d" %(j), xy=(k-0.5,j))
						k=k+1

				title("%s" %(self.pnames[i-1]))
				ylim(-0.5,1.5)
				xlim(-1, len(p))
				i=i+1

			if self.pclk:
				subplot(plts+1,1,i)
				num = range(len(self.clk))
				step(num,self.clk)

				if self.pannotate:
					kk=0
					for jj in self.clk:
						annotate("%d" %(jj), xy=(kk-0.5,jj))
						kk=kk+1
				title("clock")
				ylim(-0.5,1.5)
				xlim(-1,len(self.clk))
				

			if self.saveplots:
				nname = str(self.saveplots)
				savefig(nname)

			show()

	def addplot (self,inpt):
		for p in inpt:
			self.plists.append(p)

	def addpname(self,inpt):
		for p in inpt:
			self.pnames.append(p)
			


			



# ==========================
# CLASS : LOGIC GATE (LG)
#		(Abstract class)
# ==========================

class LG :
    # Logic Circuits have names and an evaluation function defined in child classes
    # They will also contain a set of inputs and outputs
    def __init__ (self, name) :
        self.name = name
    def evaluate (self) : return
         

# =============
# BASIC GATES :
# =============


# ==========================
# CLASS	:	NOT GATE (Not)
# ==========================
class Not (LG) :         # Inverter. Input A. Output B.
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A = Connector(self,'A', activates=1)
        self.B = Connector(self,'B')
    def evaluate (self) : self.B.set(not self.A.value)



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


# ============================
# CLASS	:	AND GATE (And)
# ============================
class And (Gate2) :       # two input AND Gate
    def __init__ (self, name) :
        Gate2.__init__ (self, name)
    def evaluate (self) : self.C.set(self.A.value and self.B.value)


# =========================
# CLASS	:	OR GATE (Or)
#==========================
class Or (Gate2) :         # two input OR gate.
    def __init__ (self, name) :
        Gate2.__init__ (self, name)
    def evaluate (self) : self.C.set(self.A.value or self.B.value)



# ===========================
# CLASS : NAND Gate (Nand)
# ===========================
class Nand (Gate2) :
	def __init__ (self, name) :
		Gate2.__init__ (self, name)
	def evaluate (self) :
		self.C.set(not(self.A.value and self.B.value))
		



# ===================================================
# DERIVED GATES:	GATES DERIVED FROM BASIC GATES
# ===================================================


# =========================
# CLASS : XOR GATE (Xor)
# =========================
class Xor (Gate2) :
    def __init__ (self, name) :
        Gate2.__init__ (self, name)
        self.A1 = And("A1") # See circuit drawing to follow connections
        self.A2 = And("A2")
        self.I1 = Not("I1")
        self.I2 = Not("I2")
        self.O1 = Or ("O1")
        self.A.connect    ([ self.A1.A, self.I2.A])
        self.B.connect    ([ self.I1.A, self.A2.A])
        self.I1.B.connect ([ self.A1.B ])
        self.I2.B.connect ([ self.A2.B ])
        self.A1.C.connect ([ self.O1.A ])
        self.A2.C.connect ([ self.O1.B ])
        self.O1.C.connect ([ self.C ])


# ==============================
# COMINATIONAL LOGIC CIRCUITS
# ==============================


# ===================================
# CLASS :	HALF ADDER (HalfAdder)
# ===================================
class HalfAdder (LG) :         # One bit adder, A,B in. Sum and Carry out
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A = Connector(self,'A',1)
        self.B = Connector(self,'B',1)
        self.S = Connector(self,'S')
        self.C = Connector(self,'C')
        self.X1= Xor("X1")
        self.A1= And("A1")
        self.A.connect    ([ self.X1.A, self.A1.A])
        self.B.connect    ([ self.X1.B, self.A1.B])
        self.X1.C.connect ([ self.S])
        self.A1.C.connect ([ self.C])


# =================================
# CLASS : FULL ADDER (FullAdder)
# =================================

class FullAdder (LG) :         # One bit adder, A,B,Cin in. Sum and Cout out
    def __init__ (self, name) :
        LG.__init__ (self, name)
        self.A    = Connector(self,'A',activates = 1)
        self.B    = Connector(self,'B',activates = 1)
        self.Cin  = Connector(self,'Cin',activates = 1)
        self.S    = Connector(self,'S')
        self.Cout = Connector(self,'Cout')
        self.H1= HalfAdder("H1")
        self.H2= HalfAdder("H2")
        self.O1= Or("O1")
        self.A.connect    ([ self.H1.A ])
        self.B.connect    ([ self.H1.B ])
        self.Cin.connect  ([ self.H2.A ])
        self.H1.S.connect ([ self.H2.B ])
        self.H1.C.connect ([ self.O1.B])
        self.H2.C.connect ([ self.O1.A])
        self.H2.S.connect ([ self.S])
        self.O1.C.connect ([ self.Cout])


# ==================================
# Mux : 2x1 MUX
# ==================================

class Mux (LG):
	def __init__(self,name):
		LG.__init__(self,name)
		self.D1 = Connector(self,'D1', activates =1)
		self.D0 = Connector(self,'D0', activates = 1)
		self.Sel  = Connector(self,'Sel', activates = 1)
		self.O = Connector(self,'O')

		self.A1 = And('A1')
		self.A2 = And('A2')
		self.N = Not('N')
		self.OR = Or('OR')

		self.D1.connect([self.A1.A])
		self.D0.connect([self.A2.A])
		self.Sel.connect([self.N.A, self.A1.B])
		self.N.B.connect([self.A2.B])
		self.A1.C.connect([self.OR.A])
		self.A2.C.connect([self.OR.B])
		self.OR.C.connect([self.O])


# =============================
# Demux : 1x2
# =============================

class Demux(LG):
	def __init__(self,name):
		LG.__init__(self,name)
		self.D = Connector(self,'D',activates = 1)
		self.Sel = Connector(self,'Sel', activates = 1)
		self.O1 = Connector(self,'O1')
		self.O0 = Connector(self,'O0')

		self.A1 = And('A1')
		self.A0 = And('A0')
		self.N = Not('N')

		self.D.connect([self.A1.A, self.A0.A])
		self.Sel.connect([self.A1.B, self.N.A])
		self.N.B.connect([self.A0.B])
		self.A0.C.connect([self.O0])
		self.A1.C.connect([self.O1])



# ======================================================================================================
#                        ************** Signal Sources ***************
# ======================================================================================================


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
		if self.skip < 0:
			ERROR_SIMU = 1
			print "Clock: %s => ERROR - skip should be non-negative, skip = %d" %(self.name, self.skip)

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
		

# ==========================================
# ConstSrc : constant source
# =========================================

class ConstSrc (LG):
	def __init__(self, name, value = 0):
		global STOP_SIMU
		global DEBUG_SIMU
		global ERROR_SIMU
		LG.__init__(self,name)
		self.value = value
		self.data = []
		self.clk_in = Connector(self,'clk_in', activates = 1)
		self.data_out = Connector(self,'data_out')
		if self.value > 1 or self.value < 0:
			STOP_SIMU = 1
			ERROR_SIMU = 1
			print "ConstSrc: %s => ERROR - value should be 1 or 0, value =%d" %(self.name, self.value)

		if DEBUG_SIMU:
			print "ConstSrc: %s => value = %d" %(self.name, self.value)

	def evaluate (self):
		self.data_out.set(self.value)
		self.data.append(self.data_out.value)
				
# ========================================
# EdgeDetector : toggle on edge
# ========================================

# Each data dets read twice if connected to istream 

class EdgeClock (LG):
	def __init__(self, name):
		LG.__init__(self,name)
		self.clk_in = Connector(self,'clk_in', activates = 1)
		self.clk_out = Connector(self,'clk_out')
		self.prev = 0

	def evaluate (self):
		if (self.prev > 0) and (self.clk_in < 1):
			self.clk_out.set(not self.clk_out.value)
		self.prev = self.clk_in.value
		
