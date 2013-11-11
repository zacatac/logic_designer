import pygame
from Socket import Socket

green = (50, 210, 50)
white = (255,255,255)
black = (0,0,0)
blue = (70,190,255)
grey = (220,220,220)
midgrey = (150,150,150)
darkgrey = (80,80,80)

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
