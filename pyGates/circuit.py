#!/usr/bin/env python

import os, sys


"""
Circuit representation after compilation

Collects each of the compositors and pairs them
with a portion of the circuit (parts).

Each of the starting points (heads of the lists)
of the search is at a switch.
The resulting search is breadth first. 
"""

class circuit:
    
    def __init__(self, gates=None, compositors=None):
        self.gates= gates #Linked List, already implemented w/ Sockets
        self.compositors = compositors # parts paired with
    def setGates(self,gates):
        self.gates = gates
    def setCompositors(self,compositors):
        self.compositors = compositors        
