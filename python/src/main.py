# Simple scipt that shows a grey screen 
# and monitors the serial port for incoming data.

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
GRAY = (82, 82, 82)
MEDIUM_GRAY = (138, 138, 138)
LIGHTGRAY = (216, 216, 216)
BRIGHT_YELLOW = (255, 230, 0)

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

# Initialize an empty dictionary
serial_dict = {}

def parseSerial(line):
    # Parse the incoming data from 'message||value' to dictionary
    if "||" in line:
        message, value = line.split("||")
        serial_dict[message] = value
        # print(f"Parsed: {serial_dict}")

# Define scene functions
def booting():
    print("arrived in booting state")
    screen.fill(BG_COLOR)
    pygame.display.update()

def idle():
    print("arrived in idle state")
    screen.fill(GRAY)
    pygame.display.update()

def challenge1():
    print("arrived in challenge1 state")
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()

def challenge1_debrief():
    print("arrived in challenge1_debrief state")
    screen.fill(LIGHTGRAY)
    pygame.display.update()

def challenge2():
    print("arrived in challenge2 state")
    screen.fill(BRIGHT_YELLOW)
    pygame.display.update()

def challenge2_debrief():
    print("arrived in challenge2_debrief state")
    screen.fill(BG_COLOR)
    pygame.display.update()

def challenge3():
    print("arrived in challenge3 state")
    screen.fill(GRAY)
    pygame.display.update()

def challenge3_debrief():
    print("arrived in challenge3_debrief state")
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()

def conclusion():
    print("arrived in conclusion state")
    screen.fill(LIGHTGRAY)
    pygame.display.update()

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
            print(line) 
            parseSerial(line)

            if "state" in serial_dict:
                scene = serial_dict["state"]
                if scene == "booting":
                    booting()
                elif scene == "idle":
                    idle()
                elif scene == "challenge1":
                    challenge1()
                elif scene == "challenge1_debrief":
                    challenge1_debrief()
                elif scene == "challenge2":
                    challenge2()
                elif scene == "challenge2_debrief":
                    challenge2_debrief()
                elif scene == "challenge3":
                    challenge3()
                elif scene == "challenge3_debrief":
                    challenge3_debrief()
                elif scene == "conclusion":
                    conclusion()

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

