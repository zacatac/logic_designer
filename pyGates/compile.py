#!/bin/env python

import pygame
import os
import sys
from load_image import load_image

"""
Compiler

Initial functionality: Organizing graphical representation
into data structure.

Next: Make a call to ViennaRNA for comparisons
"""

import Queue

def print_parts(gates):
    for gate in gates:
        print "GATE:  type: %s" %gate.type
        for inSocket in gate.inSockets:
            print "IN SOCKET: Type: %s, Gate: %s, State: %s, Index: %s"\
            %(inSocket.type, inSocket.gate.type, inSocket.state,inSocket.index)
        if gate.type == 'not' or gate.type == 'switch':
            outSocket = gate.outSocket
            print "OUT SOCKET: Type: %s, Gate: %s, State: %s, Index: %s \n"\
            %(outSocket.type, outSocket.gate.type, outSocket.state,outSocket.index)
        else: print ''

def print_circuit(gates):
    switches = []
    bulbs = []
    gates = []
    for gate in gates:
        if gate.type == 'switch':
            switches.append(gate)
    for switch in switches:
        DFS_switch(switch,gates,switches)

def BFS(switches):
    q = Queue()
    for switch in switches:
        q.put([1,switch])
    
    depth = 1
    while not q.empty():
        nxt = q.get()
        if nxt[0] == depth:
            if nxt.type == 'bulb':
                a = 1
                # There are no outsockets.
                # Add an output signal (still requires ribozyme, then xFP)
            else:
                q.put(nxt.outSocket.gate)
                #Call make function here on nxt.
        else:
            depth += 1
            
        
    
    

def graph_to_linked(gates):
    """
    Maps the graphical circuit representation to 
    a linked list
    """
    switches = []
    for gate in gates:
        if gate.type == 'switch': # For switched circuits
            switches.append(gate)
    if len(switches) == 0: #For cyclic unswitched circuits
        switches.append(gates[0])
    return switches
