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

# fonts
font_H1 = pygame.font.Font(None, 74)

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

# initatialize scene variable
scene = BOOTING

def parseSerial(line):
    # Parse the incoming data from 'message||value' to dictionary
    if "||" in line:
        message, value = line.split("||")
        serial_dict[message] = value
        # print(f"Parsed: {serial_dict}")

# Define scene functions
def booting():
    screen.fill(BG_COLOR)
    pygame.display.update()

def idle():
    screen.fill(GRAY)
    pygame.display.update()

def challenge1():
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()

def challenge1_debrief():
    screen.fill(LIGHTGRAY)
    pygame.display.update()

def challenge2():
    screen.fill(BRIGHT_YELLOW)
    pygame.display.update()

def challenge2_debrief():
    screen.fill(BG_COLOR)
    pygame.display.update()

def challenge3():
    screen.fill(GRAY)
    pygame.display.update()

def challenge3_debrief():
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()

def conclusion():
    screen.fill(LIGHTGRAY)
    pygame.display.update()

# initialization
clock = pygame.time.Clock()

# Initial background load
screen.fill(BG_COLOR)
pygame.display.update()

# Main loop
running = True
current_state = None

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False

     # Read from the serial port
    try:
        if ser and ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            # print(line) 
            parseSerial(line)

            if "state" in serial_dict:
                new_state = serial_dict["state"]
                scene = serial_dict["state"]
                if new_state != current_state:
                    current_state = new_state
                    print(f"New state: {current_state}")
                    if scene == "BOOTING":
                        booting()
                    elif scene == "IDLE":
                        idle()
                    elif scene == "CHALLENGE1":
                        challenge1()
                    elif scene == "CHALLENGE1_DEBRIEF":
                        challenge1_debrief()
                    elif scene == "CHALLENGE2":
                        challenge2()
                    elif scene == "CHALLENGE2_DEBRIEF":
                        challenge2_debrief()
                    elif scene == "CHALLENGE3":
                        challenge3()
                    elif scene == "CHALLENGE3_DEBRIEF":
                        challenge3_debrief()
                    elif scene == "CONCLUSION":
                        conclusion()

    except (serial.SerialException, OSError) as e:
        print(f"Serial connection error: {e}")
        ser = None
        # Restart the serial connection thread
        serial_thread = threading.Thread(target=connect_serial)
        serial_thread.daemon = True
        serial_thread.start()

    
    if scene == "BOOTING":
        # main title
        text = font_H1.render("Booting ...", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "IDLE":
        # main title
        text = font_H1.render("Idle state", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CHALLENGE1":
        # main title
        text = font_H1.render("Challenge 1", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()


    
    # Cap the frame rate
    clock.tick(FRAMERATE)

# Quit Pygame
if ser:
    ser.close()
pygame.quit()
sys.exit()

