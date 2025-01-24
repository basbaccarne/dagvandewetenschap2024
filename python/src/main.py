# Load libraries
import pygame
import serial
import time
import sys
import os

# Initialize Pygame
pygame.init()

# Constants
FRAMERATE = 60
BG_COLOR = (34, 34, 34)

# Set up the display
screen = pygame.display.set_mode((0,0), pygame.FULLSCREEN | pygame.DOUBLEBUF, 8)
pygame.display.set_caption("PunchPal")

# Hide the mouse cursor
pygame.mouse.set_visible(False)

# set-up the serial connection
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)

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

    # Read from the serial port
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)  # Print the received data

    # Cap the frame rate
    clock.tick(FRAMERATE)

# Quit Pygame
ser.close()
pygame.quit()
sys.exit()

