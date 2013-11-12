import pygame,os
from Socket import Socket
from load_image import load_image
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
        self.image, self.rect = load_image(os.getcwd()+"/assets/bulb0.png", -1)
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
            self.image, self.rect = load_image(os.getcwd()+"/assets/bulb1.png", -1)
            self.state = True
        else:
            self.image, self.rect = load_image(os.getcwd()+"/assets/bulb0.png", -1)
            self.state = False
        self.rect.topleft = self.cords
    def update(self):
        for sock in self.inSockets: #There's only one, I jsut added this so the program wouldn't crash when there  isn't one for some reason (could also have an if len(self.inSockets), or try(...) )
            sock.update()
        self.updateState()
        self.screen.blit(self.image,(self.rect.topleft))
