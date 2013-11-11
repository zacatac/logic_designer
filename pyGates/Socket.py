import pygame

green = (50, 210, 50)
white = (255,255,255)
black = (0,0,0)
blue = (70,190,255)
grey = (220,220,220)
midgrey = (150,150,150)
darkgrey = (80,80,80)

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
