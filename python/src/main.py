# Load libraries
import pygame
import sys
import os

# Initialize Pygame
pygame.init()

# Constants
FRAMERATE = 60
BG_COLOR = (34, 34, 34)

# Set up the display
screen = pygame.display.set_mode((0,0), pygame.FULLSCREEN, pygame.DOUBLEBUF, 8)
pygame.display.set_caption("PunchPal")

# Hide the mouse cursor
pygame.mouse.set_visible(False)

# initialization
clock = pygame.time.Clock()

# Initial background load
screen.fill(BG_COLOR)
pygame.display.update()

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False


    # Cap the frame rate
    clock.tick(FRAMERATE)

# Quit Pygame
pygame.quit()
sys.exit()

