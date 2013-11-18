



# ========================================================================
#  g a t e s . p y
#
#  Logic Circuit Simulator 
#
#  Define classes for	Connector, 
#						Logic Gates ( AND, OR, NOT, XOR),
#						Combinational Circuits (HalfAdder, FullAdder)
#						Stream (input/output bit streams)
#						Plotting
#
# =======================================================================

# ===============
# GLOBALS :
# ===============

STOP_SIMU = 0	# STOP SIGNAL
DEBUG_SIMU = 0	# DEBUG OPTION ON/OFF


# ====================
# CLASS	:	Connector
# ====================

class Connector :
    # Connectors are inputs and outputs. Only outputs should connect
    # to inputs. Be careful NOT to have circular references
    # As an output is changed it propagates the change to its connected inputs

    def __init__ (self, owner, name, activates=0, monitor=0) :
        self.value = None
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
            print "Connector %s-%s set to %d" % (self.owner.name,self.name,self.value)	
        for con in self.connects : con.set(value)


#===================
# CLASS : STREAM
# ==================

class Ostream:

	#	pin - connected to o/p pin
	#	toggle clock to change o/p
	
	def __init__(self, name,stream = 0) :
		self.name = name
		self.data = []	# output data
		self.stream = stream
		self.clk_in = Connector(self,'clk_in', activates =1)
		self.data_in = Connector(self,'data_in')

	def evaluate (self):
		global STOP_SIMU
		global DEBUG_SIMU
		if self.clk_in and self.stream and (not STOP_SIMU):
			self.data.append(int(self.data_in.value))
			if(DEBUG_SIMU) :
				print "Ostream-%s => data = %d , buffer_data =" %(self.name, self.data_in.value)
				print ''.join([str(item) for item in self.data])
				print "\n\n"




class Istream:
	"""  This is Istream class 	"""

	def __init__ (self,name,fname, stream = 0):
		self.name = name
		self.fname = fname
		self.stream = stream
		self.clk_in = Connector(self,'clk_in', activates = 1)
		self.data_out = Connector(self,'data_out')
		self.data = self.ReadFile()
		self.data_curr = 0
		self.data_max = len(self.data)

	def ReadFile(self):
		# read self.fname and convert to list ( with '\n' as last item
		f = open(self.fname)
		d = f.read()
		data = []
		for n in d:
			data.append(n)
		return data	



	def evaluate (self):
		global STOP_SIMU
		if self.clk_in  and self.stream  and (self.data_curr < self.data_max -1) and (self.data_max > 0) and (not STOP_SIMU):
			if (DEBUG_SIMU):
				print "Istream-%s => data = %d, data_curr = %d, data_max = %d" %(self.name, int(self.data[self.data_curr]), self.data_curr+1, self.data_max-1)
			self.data_out.set(int(self.data[self.data_curr]))
			self.data_curr = self.data_curr + 1
		else:
			if (DEBUG_SIMU):
				print "Istream-%s => Data Exhausted..." %(self.name)
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

	def __init__ (self,name, debug = 0, start = 0, step = 0, plots = 0):
		global DEBUG_SIMU
		self.start = start
		self.step = step
		self.name = name
		self.debug = debug
		self.plots = plots
		self.clk_out = Connector(self,'clk_out')
		DEBUG_SIMU = self.debug
		
	def simulate (self):
		if self.start :
			if (DEBUG_SIMU):
				print "\n\n\n"
				print "********************************************"
				print "* pydlcs - Digital Logic Circuit Simulator *"
				print "********************************************"

				print("Simulation Started...\n")
			self.clk_out.set(0)	
			while True :
				if (STOP_SIMU):
					if (DEBUG_SIMU):
						print("Simulation Ended ...\n")
					break
				else:
					self.clk_out.set(not self.clk_out.value)
					if (DEBUG_SIMU):
						print"SIMU-%s => clk_out=%d" %(self.name, self.clk_out.value)
		else:
			print " Please set start flag : %s.start=%d\n" %(self.name, self.start)
	

	def evaluate (self):
		pass	
			



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
        self.A    = Connector(self,'A',1,monitor=1)
        self.B    = Connector(self,'B',1,monitor=1)
        self.Cin  = Connector(self,'Cin',1,monitor=1)
        self.S    = Connector(self,'S',monitor=1)
        self.Cout = Connector(self,'Cout',monitor=1)
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


