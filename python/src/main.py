# Load libraries
import pygame
import serial
import time
import sys
import os
import threading

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
# Global variable to store the serial connection
ser = None
def connect_serial():
    global ser
    while True:
        try:
            ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) 
            print("Serial connection established")
            break
        except serial.SerialException:
            print("Failed to connect to serial port. Retrying...")
            time.sleep(2)

# Start the serial connection thread
serial_thread = threading.Thread(target=connect_serial)
serial_thread.daemon = True
serial_thread.start()

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
    try:
        if ser and ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)  # Print the received data
    except (serial.SerialException, OSError) as e:
        print(f"Serial connection error: {e}")
        ser = None
        # Restart the serial connection thread
        serial_thread = threading.Thread(target=connect_serial)
        serial_thread.daemon = True
        serial_thread.start()

    # Cap the frame rate
    clock.tick(FRAMERATE)

# Quit Pygame
if ser:
    ser.close()
pygame.quit()
sys.exit()

