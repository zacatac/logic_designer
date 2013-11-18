#!/usr/bin/env python

import os, sys


"""
Class representation of ribozyme

"""

class compositor:
    
    def __init__(self, name=None, type=None, seq=None):
        self.name = name
        self.type = type
        self.seq = seq        
    def set_name(self, name):
        self.name = name
    def set_seq(self, seq):
        self.seq = seq
    def set_type(self, type):
        self.type = type

