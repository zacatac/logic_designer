import pygame,os
from load_image import load_image
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
