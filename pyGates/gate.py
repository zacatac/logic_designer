import os,pygame
from load_image import load_image
from Socket import Socket

green = (50, 210, 50)
white = (255,255,255)
black = (0,0,0)
blue = (70,190,255)
grey = (220,220,220)
midgrey = (150,150,150)
darkgrey = (80,80,80)

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
        self.image, self.rect = load_image(os.getcwd()+"/assets/"+gateType+'.png', -1)
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
        timedelay = 25 # actual time delay
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
