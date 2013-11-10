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

green = (50, 210, 50)
white = (255,255,255)
black = (0,0,0)
blue = (70,190,255)
grey = (220,220,220)
midgrey = (150,150,150)
darkgrey = (80,80,80)

def load_image(name, colorkey=None):
    fullname = os.path.join('data', name)
    print(name)
    try:
        image = pygame.image.load(fullname)
    except pygame.error, message:
        print 'Cannot load image:', fullname
        raise SystemExit, message
    image = image.convert()
    if colorkey is not None:
        if colorkey is -1:
            colorkey = image.get_at((0,0))
            image.set_colorkey(colorkey, pygame.RLEACCEL)
    return image, image.get_rect()
class gateCreator(pygame.sprite.Sprite):
    """
        These are the buttons on the side of the screen to create the gates
    """
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.type = None
        self.image = None
        self.screen = None
    def setScreen(self, screen):
        self.screen = screen
        self.screen.blit(self.image,(self.rect.topleft))
    def pressed(self,mouse):    #this function appears in all the gate-like classes, it just checks if the mouse is on the button and is called whenever theres a mousedown.
        if mouse[0] > self.rect.topleft[0]:
            if mouse[1] > self.rect.topleft[1]:
                if mouse[0] < self.rect.bottomright[0]:
                    if mouse[1] < self.rect.bottomright[1]:
                        return True
                    else: return False
                else: return False
            else: return False
        else: return False
    def setCords(self,x,y):
        self.rect.topleft = x,y
    def draw(self):
        self.screen.blit(self.image,(self.rect.topleft))
    def setType(self, gateType):
        self.type = gateType
        if self.type == "switch":
            self.image, self.rect = load_image(os.getcwd()+"/"+gateType+'0.bmp', -1) #bulbs and switches have two images each
        elif self.type == "bulb":
            self.image, self.rect = load_image(os.getcwd()+"/"+gateType+'0.png', -1)
        else:
            self.image, self.rect = load_image(os.getcwd()+"/"+gateType+'.png', -1)
class switch(pygame.sprite.Sprite):
    """
        Class for the True/False switch. Contains a lot of methods from the gate class, in fact it may have been possible to integrate it with it, but I didn't want the code to get too messy.
    """
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.image = None
        self.inSockets = [] #always a list, even if theres just one input -> makes the rest of the code easier.
        self.outSocket = None
        self.state = False
        self.lastState = False
        self.cords = (0,0)
        self.pushed = False
    def create(self, gateType, mouseCords,screen):
        self.screen = screen
        self.type = gateType
        self.image, self.rect = load_image(os.getcwd()+"/switch0.bmp", -1)
        print "switch created, cords: ",self.rect.topleft
        self.setCords(mouseCords[0],mouseCords[1])
        outSocket = Socket()
        outSocket.setType("out")
        outSocket.setGate(self)
        outSocket.setScreen(self.screen)
        outSocket.update()
        self.outSocket = outSocket
        print "switch created at ", mouseCords," with outSocket at ", outSocket.rect.topleft
    def SHUTDOWNEVERYTHING(self):   #called when the object is deleted. Here we attempt to destroy all sockets and cables attached to the gate (this works the same way in the actual gate class)
        self.image, self.rect = None, None
        for cab in self.outSocket.attachedCables:
            cab.startSocket.attachedCables.remove(cab)
            cab.endSocket.attachedCables.remove(cab)
            cab.setStartSocket(None)
            cab.setEndSocket(None)
        self.outSocket.attachedCables = []
        self.outSocket = None
    def setCords(self,x,y):
        self.rect.topleft = x,y
        self.cords = (x,y)
        self.screen.blit(self.image,(self.rect.topleft))
    def push(self):     #change the switch state and load the appropriate image
        if self.outSocket.state == True:
            self.outSocket.setState(False)
            self.image, self.rect = load_image(os.getcwd()+"/switch0.bmp", -1)
            self.rect.topleft = self.cords
        else:
            self.outSocket.setState(True)
            self.image, self.rect = load_image(os.getcwd()+"/switch1.bmp", -1)
            self.rect.topleft = self.cords
        self.outSocket.update()
    def pressed(self,mouse):
        if mouse[0] > self.rect.topleft[0]:
            if mouse[1] > self.rect.topleft[1]:
                if mouse[0] < self.rect.bottomright[0]:
                    if mouse[1] < self.rect.bottomright[1]:
                        if self.outSocket.pressed(mouse):
                            return "out"
                        self.push()
                        return "main"
                    else: return False
                else: return False
            else: return False
        else: return False
    def update(self):
        self.screen.blit(self.image,(self.rect.topleft))
        self.outSocket.update() #we always update sockets from within the gate classes' update method.
class bulb(pygame.sprite.Sprite):
    """
        Class for the lightbulb. Again, much of this looks just like in the gate class.
    """
    ## This code should be pretty self-explanatory.
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.type = None
        self.image = None
        self.state = False
        self.inSockets = []
    def create(self, gateType, mouseCords,screen):
        self.screen = screen
        self.type = gateType
        self.image, self.rect = load_image(os.getcwd()+"/bulb0.png", -1)
        print self.type," created, cords: ",self.rect.topleft
        self.setCords(mouseCords[0],mouseCords[1])
        socket = Socket()
        socket.setType("in")
        socket.setGate(self)
        socket.setIndex(0)
        socket.setScreen(self.screen)
        socket.update()
        self.inSockets.append(socket)
    def SHUTDOWNEVERYTHING(self):
        self.image, self.rect = None, None
        for cab in self.inSockets[0].attachedCables:
            cab.startSocket.attachedCables.remove(cab)
            cab.setStartSocket(None)
            cab.setEndSocket(None)
        self.inSockets[0].attachedCables = []
    def setCords(self,x,y):
        self.rect.topleft = x,y
        self.screen.blit(self.image,(self.rect.topleft))
        self.cords = (x,y)
    def pressed(self,mouse):
        if mouse[0] > self.rect.topleft[0]:
            if mouse[1] > self.rect.topleft[1]:
                if mouse[0] < self.rect.bottomright[0]:
                    if mouse[1] < self.rect.bottomright[1]:
                        for socket in self.inSockets:
                            if socket.pressed(mouse):
                                return "in"+str(socket.index)
                        return "main"
                    else: return False
                else: return False
            else: return False
        else: return False
    def updateState(self):
        if self.inSockets[0].state:
            self.image, self.rect = load_image(os.getcwd()+"/bulb1.png", -1)
            self.state = True
        else:
            self.image, self.rect = load_image(os.getcwd()+"/bulb0.png", -1)
            self.state = False
        self.rect.topleft = self.cords
    def update(self):
        for sock in self.inSockets: #There's only one, I jsut added this so the program wouldn't crash when there  isn't one for some reason (could also have an if len(self.inSockets), or try(...) )
            sock.update()
        self.updateState()
        
        self.screen.blit(self.image,(self.rect.topleft))
class gate(pygame.sprite.Sprite):
    """
        The main class for creating the different logic gates.
    """
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.type = None
        self.image = None
        self.inSockets = []
        self.outSocket = None
        self.updateStateDelay = 0
    def create(self, gateType, mouseCords,screen):
        self.screen = screen
        self.type = gateType
        self.image, self.rect = load_image(os.getcwd()+"/"+gateType+'.png', -1)
        print self.type," created, cords: ",self.rect.topleft
        self.setCords(mouseCords[0],mouseCords[1])
        if gateType == "not": #for a not gate, we only create one input socket.
            socket1 = Socket()
            socket1.setType("in")
            socket1.setGate(self)
            socket1.setIndex(0)
            socket1.setScreen(self.screen)
            socket1.update()
            self.inSockets.append(socket1)

            outSocket = Socket()
            outSocket.setType("out")
            outSocket.setGate(self)
            outSocket.setScreen(self.screen)
            outSocket.update()
            self.outSocket = outSocket
            
        else:   #otherwise: two inputs, one output.
            socket1 = Socket()
            socket1.setType("in")
            socket1.setGate(self)
            socket1.setIndex(0)
            socket1.setScreen(self.screen)
            socket1.update()
            self.inSockets.append(socket1)

            socket2 = Socket()
            socket2.setType("in")
            socket2.setGate(self)
            socket2.setIndex(1)
            socket2.setScreen(self.screen)
            socket2.update()
            self.inSockets.append(socket2)

            outSocket = Socket()
            outSocket.setType("out")
            outSocket.setGate(self)
            outSocket.setScreen(self.screen)
            outSocket.update()
            self.outSocket = outSocket
    def SHUTDOWNEVERYTHING(self): #self-destruction method for when the gate is deleted
        self.image, self.rect = None, None
        for cab in self.outSocket.attachedCables:
            if cab in cab.startSocket.attachedCables:
                cab.startSocket.attachedCables.remove(cab)
            if cab in cab.endSocket.attachedCables:
                cab.endSocket.attachedCables.remove(cab)
            cab.startSocket.setState(False)
            cab.endSocket.setState(False)
            cab.setStartSocket(None)
            cab.setEndSocket(None)
        self.outSocket.attachedCables = []
        self.outSocket = None
        self.inSockets = []
    def setCords(self,x,y):
        self.rect.topleft = x,y
        self.screen.blit(self.image,(self.rect.topleft))
    def pressed(self,mouse):
        if mouse[0] > self.rect.topleft[0]:
            if mouse[1] > self.rect.topleft[1]:
                if mouse[0] < self.rect.bottomright[0]:
                    if mouse[1] < self.rect.bottomright[1]:
                        for socket in self.inSockets:
                            if socket.pressed(mouse):
                                return "in"+str(socket.index)
                        if self.outSocket.pressed(mouse):
                            return "out"
                        return "main"
                    
                    else: return False
                else: return False
            else: return False
        else: return False
    def updateState(self): # interesting aside, I'm pretty sure this whole thing is Turing complete
        timedelay = 10 # actual time delay
        print self.updateStateDelay
        if self.updateStateDelay >= timedelay:
            self.updateStateDelay = 0
            if self.type == "or":
                self.outSocket.setState(self.inSockets[0].state or self.inSockets[1].state)
            elif self.type == "and":
                self.outSocket.setState(self.inSockets[0].state and self.inSockets[1].state)
            elif self.type == "xor":
                self.outSocket.setState(self.inSockets[0].state ^ self.inSockets[1].state)
            elif self.type == "xnor":
                self.outSocket.setState(not (self.inSockets[0].state) ^ (self.inSockets[1].state))
            elif self.type == "nor":
                self.outSocket.setState(not(self.inSockets[0].state or self.inSockets[1].state))
            elif self.type == "nand":
                self.outSocket.setState(not(self.inSockets[0].state and self.inSockets[1].state))
            elif self.type == "not":
                self.outSocket.setState(not self.inSockets[0].state)
            else:
                pass
        else:
            self.updateStateDelay += 1
    def update(self):
        for sock in self.inSockets:
            sock.update()
        self.updateState()
        self.outSocket.update()
        self.screen.blit(self.image,(self.rect.topleft))

class Socket(pygame.sprite.Sprite):
    """
        Socket class. One of these is instantiated for each of the connectors on each logic gate.  
    """
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.rect = None
        self.type = None        #input or output
        self.index = None
        self.state = False
        self.cords = ()
        self.connected = False
        self.gate = None        #which gate is the socket attached to?
        self.attachedCables = []
    def setScreen(self,screen):
        self.screen = screen
        self.rect = pygame.draw.rect(self.screen, black,(0,0,10,10))
    def setGate(self,gate):
        self.gate = gate
    def setState(self, state):
        self.state = state
    def setType(self,Type):
        self.type = Type
    def setIndex(self,index):
        self.index = index
    def pressed(self,mouse):
        if mouse[0] > self.rect.topleft[0]:
            if mouse[1] > self.rect.topleft[1]:
                if mouse[0] < self.rect.bottomright[0]:
                    if mouse[1] < self.rect.bottomright[1]:
                        return True
                    else: return False
                else: return False
            else: return False
        else: return False
    def connect(self, cable):   #connect a cable to the socket
        if self.type == "in":
            if self.connected:  #can't have multiple inputs
                return False
            else:
                self.attachedCables = [cable]
        else:                   # CAN have multiple outputs
            self.attachedCables.append(cable)
        self.connected = True
        return True
    def disconnect(self, cable):    #remove self from whatever gates it is attached to
        if self.type == "in":
            self.state = False
        self.attachedCables.remove(cable)
        self.connected = False
    def update(self):
        if self.type == "in":
            if self.index == 0: # if index 0 -> socket is the upper left input (or center).
                if self.gate.type == "not" or self.gate.type == "bulb":
                    self.rect.midleft = self.gate.rect.midleft  # the inputs for bulbs and inverters are centered, not top and bottom left.
                    save = self.rect.topleft
                else:   
                    self.rect.topleft = (self.gate.rect.topleft[0],self.gate.rect.topleft[1]+10) # we add 10 pixels so the connector is on the symbols wire, not the edge of the picture.
                    save = self.rect.topleft
            else:   # index is 1, so the socket is on the bottom left side.
                self.rect.bottomleft = (self.gate.rect.bottomleft[0],self.gate.rect.bottomleft[1]-10)
                save = self.rect.topleft
        else:
            self.rect.midright = self.gate.rect.midright
            save = self.rect.topleft
        for cable in self.attachedCables: #move attached cables
            if self.type == "in":
                if cable.update():
                    pass
                else:
                    self.connected = False
        if not (len(self.attachedCables)):  #no cables attached? better set self.connected to false. And self.state too, but only if it's an input (since empty outputs can still return True)
            if self.type == "in":
                self.state = False
            self.connected = False
        if self.state:
            if len(self.attachedCables) or self.type == "out":
                self.rect = pygame.draw.rect(self.screen, blue,(save[0],save[1],10,10))
            else:
                self.rect = pygame.draw.rect(self.screen, black,(save[0],save[1],10,10))
        else:
            self.rect = pygame.draw.rect(self.screen, black,(save[0],save[1],10,10))
       
class cable(pygame.sprite.Sprite):
    """
        Cable class, transfers logic vales from  socket to socket.
    """
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.startSocket= None
        self.endSocket = None
        self.state = None
    def setScreen(self,screen):
        self.screen = screen
        self.startRect = pygame.draw.rect(self.screen, green,(0,0,10,10))
        self.endRect = pygame.draw.rect(self.screen, green,(0,0,10,10))
    def setStartSocket(self,sock): 
        self.startSocket = sock
        if sock: #tehee, sock
            self.startRect.center = sock.rect.center
    def setEndSocket(self, sock):
        self.endSocket = sock
        if sock:
            self.endRect.center = sock.rect.center
    def disconnect(self, end):
        if end == "in":
            self.endSocket.disconnect(self)
        else:
            self.startSocket.disconnect(self)
    def drawBezier(self,start, end, color): #just a method to make drawing the curves easier in the next part, since we're gonna be drawing a bunch of them.
        steps = 20  #this seems to affect how smooth the curve will be, but might affect performance if its set too high. 
        mid = ((start[0]+end[0])/2,(start[1]+end[1])/2.0+(0.5*abs(start[0]-end[0])))    #this changes the way the cables will actually curve. Might improve it at some point but it works decently well as-is.
        points = (start,mid,end)
        pygame.gfxdraw.bezier(self.screen, [(point[0],point[1]) for point in points],  steps, color)
    def drawCable(self, startCords, endCords):
        if self.startSocket:
            if self.startSocket.state == True:  # we're drawing either from an output to input, or output to mouse. either way, the cable might be carrying True
                for x in range(4):
                    for y in range(4):
                        for x1 in range(4):
                            for y1 in range(4):
                                self.drawBezier(((startCords[0]+x),(startCords[1]+y)), ((endCords[0]+x1,endCords[1]+y1)), black)    #draw a bunch of black beziers
                for x in range(1,3):
                    for y in range(1,3):
                        for x1 in range(1,3):
                            for y1 in range(1,3):
                                self.drawBezier(((startCords[0]+x),(startCords[1]+y)), ((endCords[0]+x1,endCords[1]+y1)), blue)     #then draw a smaller bunch of blue ones on top.
            else:
                for x in range(4):
                    for y in range(4):
                        self.drawBezier(((startCords[0]+x),(startCords[1]+y)), ((endCords[0]+x,endCords[1]+y)), black)  #same deal, except...
                for x in range(1,3):
                    for y in range(1,3):
                        self.drawBezier(((startCords[0]+x),(startCords[1]+y)), ((endCords[0]+x,endCords[1]+y)), white)  #.. we now draw white beziers on top.
        else:       #this is in case the cable is drawn from an endSocket to the mouse pointer.
            for x in range(4):
                for y in range(4):
                    self.drawBezier(((startCords[0]+x),(startCords[1]+y)), ((endCords[0]+x,endCords[1]+y)), black)
            for x in range(1,3):
                for y in range(1,3):

                    self.drawBezier(((startCords[0]+x),(startCords[1]+y)), ((endCords[0]+x,endCords[1]+y)), white)
    
    def update(self):
        if self.startSocket and self.endSocket:
            self.startRect.center = self.startSocket.rect.center
            self.endRect.center = self.endSocket.rect.center
            self.endSocket.setState(self.startSocket.state) #transfer signal
            self.drawCable(self.startRect.center,self.endRect.center)
            return True
        else:
            return False

def main():
    def drawScreen():   #updates all the gates (and this also sockets and cables), then draws them to the screen.
        pygame.draw.line(screen, black,(100,0),(100,screensize[1]),2)
        for Gate in gates:
            Gate.update()
        for creator in creators:
            creator.draw()
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
        creator.setCords(0,index*50)#creator.rect.height)
        creator.setScreen(screen)

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
