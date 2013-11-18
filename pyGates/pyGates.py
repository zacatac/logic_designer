#/usr/bin/env python
# -*- coding: cp1252 -*-

"""
A simple Logic Gate simulator.

    - To create a gate, simply drag whatever gate you want from the left side of the screen. Gates are connected by clicking on the appropriate sockets.
    - Note that you may only connect input sockets to output sockets and vice versa.
    - Delete gates by dragging them back to the left side of the screen.
    - Wires may be removed from input sockets by clicking them again.
    - An output socket can have many wires attached to it, input sockets of course can only have one.
    - Wires coming from output sockets can only be removed from the input socket they end at.

www.home.in.tum.de/~dowling/ 

by Philipp Dowling, Technische Universität München 2011

"""

import pygame
import pygame.gfxdraw
import os
import time
import sys

from load_image import load_image
from gateCreator import gateCreator
from cable import cable
from Socket import Socket
from gate import gate
from switch import switch
from bulb import bulb
from button import button
from compile import print_circuit as pc, print_parts as pp

green = (50, 210, 50)
white = (255,255,255)
black = (0,0,0)
blue = (70,190,255)
grey = (220,220,220)
midgrey = (150,150,150)
darkgrey = (80,80,80)
       
def main():
    def drawScreen():   #updates all the gates (and this also sockets and cables), then draws them to the screen.
        pygame.draw.line(screen, black,(250,0),(250,screensize[1]),2)
        for Gate in gates:
            Gate.update()
        for creator in creators:
            creator.draw()
        for button in buttons:
            button.draw()
    pygame.init()
    screensize = (1200,700)
    screen = pygame.display.set_mode(screensize)
    
    # The FPS the simulation will run at.
    FPS = 30
    sleeptime = 1/float(FPS) #default sleep time


    creatorTypes = ["and","or","xor","nand","nor","xnor","not","switch","bulb"]

    gates = []  #in a sense, this is LIFO when it comes to drawing: the last element will be drawn on top.
    cables = []
    creators = [gateCreator() for x in range(len(creatorTypes))]
    
    for index, creator in enumerate(creators):
        creator.setType(creatorTypes[index])
        creator.setCords(75,index*50)#creator.rect.height)
        creator.setScreen(screen)
    
    compiler = button()
    compiler.setType("compile")
    compiler.setCords(10,len(creatorTypes)*50) # under the creators
    compiler.setScreen(screen)
    buttons = [compiler]

    running = True

    while running: #each iteration of this is one frame, unless gates are clicked (framerate will remain the same however)
        time.sleep(sleeptime)   #wait a little on each frame
        events = pygame.event.get()
        screen.fill(white)
        drawScreen()
        pygame.display.flip()
        for g in gates: #check if any gates should be deleted
            if g.rect.center[0]<100:
                g.SHUTDOWNEVERYTHING()
                gates.remove(g)
                    
        for event in events:                #handle events
            if event.type == 2 and event.key == 27:     #escape key pressed? -> quit
                running = False
            
            if event.type == pygame.MOUSEBUTTONDOWN:    #mouse pressed?
                mouse = pygame.mouse.get_pos()
                mousePressed = True
                for gC in creators:                     #check if creators have been pressed  -  if so, create gate.
                    if gC.pressed(mouse):
                        if gC.type == "bulb":
                            Gate = bulb()
                            cOffset = (gC.rect.topleft[0] - mouse[0], gC.rect.topleft[1]-mouse[1]) 
                            Gate.create(gC.type,(mouse[0]+cOffset[0],mouse[1]+cOffset[1]),screen)
                            gates.append(Gate)
                        elif gC.type == "switch":
                            Gate = switch()     #HAHA! It's not actually a gate!
                            cOffset = (gC.rect.topleft[0] - mouse[0], gC.rect.topleft[1]-mouse[1]) #mouse offset - we want to be able to drag the gate from wherever we press it
                            Gate.create(gC.type,(mouse[0]+cOffset[0],mouse[1]+cOffset[1]),screen)
                            gates.append(Gate)
                        else:
                            Gate = gate()
                            cOffset = (gC.rect.topleft[0] - mouse[0], gC.rect.topleft[1]-mouse[1]) #mouse offset - we want to be able to drag the gate from wherever we press it
                            Gate.create(gC.type,(mouse[0]+cOffset[0],mouse[1]+cOffset[1]),screen)
                            gates.append(Gate)
                    
                        offset = (Gate.rect.topleft[0] - mouse[0], Gate.rect.topleft[1]-mouse[1])
                        
                        while pygame.MOUSEBUTTONDOWN not in [ev.type for ev in pygame.event.get()]:
                            mouse = pygame.mouse.get_pos()
                            screen.fill(white)
                            Gate.setCords(mouse[0]+offset[0],mouse[1]+offset[1])
                            drawScreen()
                            pygame.display.flip()
                for b in buttons:
                    if b.pressed(mouse):
                        print "%s clicked" %b
                        if b.type == 'compile':
                            pc(gates)
                #### FOR THE LOVE OF ALL THAT IS HOLY DO NOT TOUCH THIS NEXT PART #####
                for Gate in gates:                      #check if gate was clicked
                    ev = pygame.event.get()
                    mouse = pygame.mouse.get_pos()
                    p = Gate.pressed(mouse)
                    if p:
                        if p == "main":         #gate was clicked "on body", not sockets so we dont need to worry about cable stuff here.
                            offset = (Gate.rect.topleft[0] - mouse[0], Gate.rect.topleft[1]-mouse[1])   #dragging offset again
                            while pygame.MOUSEBUTTONUP not in [ev.type for ev in ev]:
                                time.sleep(sleeptime)
                                mouse = pygame.mouse.get_pos()
                                Gate.setCords(mouse[0]+offset[0],mouse[1]+offset[1])
                                screen.fill(white)
                                drawScreen()
                                ev = pygame.event.get()
                                pygame.display.flip()
                            
                        elif p[:2] == "in":         # oh boy, someone clicked an input socket, now what do we do
                            
                            ### Disconnect logic - only inputs can be disconnected
                            if Gate.inSockets[int(p[2:])].connected:    #great, the socket already had a connection on it. lets change that..
                                print "disconnecting"
                                Cable = Gate.inSockets[int(p[2:])].attachedCables[0]
                                Cable.disconnect("in")
                                mousePressed = False
                                gatePressed = False
                                while not mousePressed:     #loop for dragging the loose cable around. also yes, I should probably have called them wires
                                        time.sleep(sleeptime)
                                        mouse = pygame.mouse.get_pos()
                                        screen.fill(white)
                                        drawScreen()
                                        Cable.drawCable(Cable.startSocket.rect.center,mouse)
                                        if pygame.MOUSEBUTTONDOWN in [ev.type for ev in pygame.event.get()]:
                                            print "mousedown"
                                            for g in gates:
                                                press = g.pressed(mouse)    #a socket was clicked, lets attach the cable here
                                                if press:
                                                    print "#############"
                                                    mousePressed = True
                                                    if press[:2]=="in":     #only worry about input sockets
                                                        print "connecting to ", g
                                                        inSock = g.inSockets[int(press[2:])]    #saving the socket for connecting to later
                                                        gatePressed = True
                                                        break
                                            if not gatePressed: #none of the gates were clicked, but the mouse was -> delete the cable.
                                                print "no gate pressed, deleting cable"
                                                mousePressed = True
                                                break
                                                    
                                        pygame.display.flip()
                                        if gatePressed: # this part could have probably gone where we check if a gate was pressed
                                            print "gate pressed after disconnect, should be reconnecting"
                                            if inSock.connect(Cable):
                                                Cable.setEndSocket(inSock)
                                                print "connected",Cable.endSocket," to ", Cable.startSocket, " with ", Cable
                                            break
                                        if mousePressed: # here we delete the cable
                                            Cable.startSocket.attachedCables.remove(Cable)
                                            Cable.endSocket.attachedCables = []
                                            Cable.startSocket = None
                                            Cable.endSocket = None
                                            break
                
                            #### connect logic for input -> outpout
                            else:   #whew, long if statement there
                                print "cable from in"
                                Cable = cable()
                                Cable.setScreen(screen)
                                Cable.setEndSocket(Gate.inSockets[int(p[2:])])
                                if Gate.inSockets[int(p[2:])].connect(Cable):       # this whole thing pretty much runs analog to the disconnect logic, just in reverse
                                    mousePressed = False
                                    gatePressed = False
                                    while not mousePressed:#(pygame.MOUSEBUTTONDOWN not in [ev.type for ev in pygame.event.get()]):
                                        time.sleep(sleeptime)
                                        mouse = pygame.mouse.get_pos()
                                        screen.fill(white)
                                        drawScreen()
                                        Cable.drawCable(mouse,Cable.endSocket.rect.center)
                                        if pygame.MOUSEBUTTONDOWN in [ev.type for ev in pygame.event.get()]:
                                            print "mousedown"
                                            for g in gates:
                                                if g.pressed(mouse)=="out":     #only connect if an output socket was clicked
                                                    print "connecting to output"
                                                    outSock = g.outSocket
                                                    gatePressed = True
                                                    break
                                                mousePressed = True
                                            if not gatePressed:
                                                print "mouse pressed, gates iterated, none pressed, disconnecting cable."
                                                Gate.inSockets[int(p[2:])].disconnect(Cable)
                                                
                                        pygame.display.flip()
                                        if gatePressed:
                                            if outSock.connect(Cable):
                                                Gate.inSockets[int(p[2:])].connect(Cable)
                                                Cable.setStartSocket(outSock)
                                                print "connected",Cable.startSocket," to ", Cable.endSocket, " with ", Cable
                                            else:
                                                Gate.inSockets[int(p[2:])].disconnect(Cable)
                                            break
                        ### connect logic for output -> input
                        else:
                            print "cable from out"
                            Cable = cable()
                            Cable.setScreen(screen)
                            Cable.setStartSocket(Gate.outSocket)
                            if Gate.outSocket.connect(Cable):
                                mousePressed = False
                                gatePressed = False
                                while not mousePressed:#(pygame.MOUSEBUTTONDOWN not in [ev.type for ev in pygame.event.get()]):
                                    time.sleep(sleeptime)
                                    mouse = pygame.mouse.get_pos()
                                    screen.fill(white)
                                    drawScreen()
                                    Cable.drawCable(mouse,Cable.startSocket.rect.center)
                                    if pygame.MOUSEBUTTONDOWN in [ev.type for ev in pygame.event.get()]: #little different here: we need to specify which input socket to connect to
                                        print "mousedown"
                                        for g in gates:
                                            press = g.pressed(mouse)    #this returns "in0" and "in1" depending on which socket was clicked
                                            print press
                                            if press =="in0":#(("in0" in [g.pressed(mouse) for g in gates])):
                                                print "connecting to input0"
                                                inSock = g.inSockets[0]
                                                gatePressed = True
                                                break
                                            elif press =="in1":#("in1" in [g.pressed(mouse) for g in gates]):
                                                print "connecting to input1"
                                                inSock = g.inSockets[1]
                                                gatePressed = True
                                                break
                                            mousePressed = True
                                        if not gatePressed:
                                            print "not connecting."
                                            Gate.outSocket.disconnect(Cable)
                                            break
                                    
                                    pygame.display.flip()
                                    if gatePressed:
                                        if inSock.connect(Cable):
                                            Cable.setEndSocket(inSock)
                                            print "connected",Cable.endSocket," to ", Cable.startSocket, " with ", Cable
                                        break
                        break
                ########################################
    pygame.quit()


if __name__ == '__main__':
    main()
