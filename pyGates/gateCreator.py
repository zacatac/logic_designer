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
            self.image, self.rect = load_image(os.getcwd()+"/assets/"+gateType+'0.bmp', -1) #bulbs and switches have two images each
        elif self.type == "bulb":
            self.image, self.rect = load_image(os.getcwd()+"/assets/"+gateType+'0.png', -1)
        else:
            self.image, self.rect = load_image(os.getcwd()+"/assets/"+gateType+'.png', -1)
