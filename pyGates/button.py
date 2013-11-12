import pygame, os, sys
from load_image import load_image

class button(pygame.sprite.Sprite):
    """
    Class for creating clickable buttons to comile, reset, etc.
    """
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.type = None
        self.image = None
        self.screen = None
    def setScreen(self,screen):
        self.screen = screen
        self.screen.blit(self.image,(self.rect.topleft))
    def pressed(self,mouse):
        if mouse[0] > self.rect.topleft[0]:
            if mouse[1] > self.rect.topleft[1]:
                if mouse[0] < self.rect.bottomright[0]:
                    if mouse[1] < self.rect.bottomright[1]:
                        return True
        else: return False
    def setCords(self,x,y):
        self.rect.topleft = x,y
    def draw(self):
        self.screen.blit(self.image,(self.rect.topleft))
    def setType(self, buttonType):
        self.type = buttonType
        if self.type == "compile":
            self.image, self.rect = load_image(os.getcwd()+"/assets/"+self.type+'.png', -1)
        elif self.type == "reset":
            self.image, self.rect = load_image(os.getcwd()+"/assets/ "+self.type+'.png',-1)
        else:
            print "Error: buttonType not recognized"
            sys.exit(0)
