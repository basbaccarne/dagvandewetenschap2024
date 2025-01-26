# Simple scipt that shows a grey screen 
# and monitors the serial port for incoming data.

# Load libraries
import pygame
import serial
import time
import sys
import os
from pathlib import Path
import threading

# Initialize Pygame
pygame.init()

# CONSTANTS
"""video settings"""
FRAMERATE = 24
screen_info = pygame.display.Info()
screen_width = screen_info.current_w
screen_height = screen_info.current_h
pygame.mouse.set_visible(False)

"""paths"""
script_dir = Path(__file__).parent
data_dir = script_dir.parent / 'data'
spritesheet_path = data_dir / 'boxer_spritesheet.png'
font1_path = data_dir / 'Jost-VariableFont_wght.ttf'
font2_path = data_dir / 'JosefinSans-VariableFont_wght.ttf'

"""design system"""
BG_COLOR = (34, 34, 34)
GRAY = (82, 82, 82)
MEDIUM_GRAY = (138, 138, 138)
LIGHTGRAY = (216, 216, 216)
BRIGHT_YELLOW = (255, 230, 0)

font_H1 = pygame.font.Font(font1_path, 100)
font_H2 = pygame.font.Font(font2_path, 50)

"""animated sprite settings"""
COL_COUNT = 14  # the spritesheet has 14 columns
ROW_COUNT  = 9  # the spritesheet has 9 rows
FPS = 24        # frames per second
SPRITE_WIDTH = 195     # each frame is 195 pixels wide
SPRITE_HEIGHT = 300    # each frame is 300 pixels high
VALID_FRAMES = (COL_COUNT*ROW_COUNT) - 5 # the last 5 frames in the spriteheet are blank

# FUNCTIONS
"""Function to load a spritesheet"""
def load_spritesheet(spritesheet_path):
    """Load and preprocess frames from the spritesheet."""
    spritesheet = pygame.image.load(spritesheet_path).convert_alpha()
    frames = []
    for row in range(ROW_COUNT):
        for col in range(COL_COUNT):
            frame_index = row * COL_COUNT + col
            if frame_index >= VALID_FRAMES:
                break
            frame = spritesheet.subsurface(
                pygame.Rect(col * SPRITE_WIDTH, row * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT)
            )
            frames.append(frame)
    return frames

"""Function to set-up the serial connection"""
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

"""Function to parse the incoming serial data"""
serial_dict = {}
def parseSerial(line):
    # Parse the incoming data from 'message||value' to dictionary
    if "||" in line:
        message, value = line.split("||")
        serial_dict[message] = value
        # print(f"Parsed: {serial_dict}")

"""Function to read from the serial port and set variables"""
def readSerial():
    global ser, current_state  
    try:
        if ser and ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            # uncomment to print raw serial input
            # print(line) 

            parseSerial(line)

            # detect state changes
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

    # manage serial connection errors
    except (serial.SerialException, OSError) as e:
        print(f"Serial connection error: {e}")
        ser = None
        # Restart the serial connection thread
        serial_thread = threading.Thread(target=connect_serial)
        serial_thread.daemon = True
        serial_thread.start()

# SCENE MANAGEMENT (static scene elements)
"""Set initial scene"""
scene = "BOOTING"

"""Function to render the booting screen"""
def booting():
    # background
    screen.fill(BG_COLOR)

    # main title
    text = font_H1.render("Syteem operationeel!", True, (MEDIUM_GRAY))
    text_rect = text.get_rect()
    text_rect.center = (screen_width // 2, screen_height // 2)
    screen.blit(text, text_rect)

    # subtitle
    text = font_H2.render("Klop op de zak om verder te gaan", True, (BRIGHT_YELLOW))
    text_rect = text.get_rect()
    text_rect.center = (screen_width // 2, screen_height // 2 + 100)
    screen.blit(text, text_rect)

    pygame.display.flip()

"""Function to render the idle screen"""
def idle():
    # background
    screen.fill(BG_COLOR)

    # main title
    text = font_H1.render("Klop om de zak om te beginnen.", True, (MEDIUM_GRAY))
    text_rect = text.get_rect()
    text_rect.center = (screen_width // 2, screen_height // 4)
    screen.blit(text, text_rect)

    # Define position for the animated sprite
    frame_x = (SCREEN_WIDTH - SPRITE_WIDTH) // 2
    frame_y = (SCREEN_HEIGHT - SPRITE_HEIGHT) // 2 + 100
    animation_rect = pygame.Rect(frame_x, frame_y, SPRITE_WIDTH, SPRITE_HEIGHT)

    pygame.display.update()

"""Function to render the challenge 1 screen"""
def challenge1():
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()

""""Function to render the challenge 1 debrief screen"""
def challenge1_debrief():
    screen.fill(LIGHTGRAY)
    pygame.display.update()

"""Function to render the challenge 2 screen"""
def challenge2():
    screen.fill(BRIGHT_YELLOW)
    pygame.display.update()

"""Function to render the challenge 2 debrief screen"""
def challenge2_debrief():
    screen.fill(BG_COLOR)
    pygame.display.update()

"""Function to render the challenge 3 screen"""
def challenge3():
    screen.fill(GRAY)
    pygame.display.update()

"""Function to render the challenge 3 debrief screen"""
def challenge3_debrief():
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()

"""Function to render the conclusion screen"""
def conclusion():
    screen.fill(LIGHTGRAY)
    pygame.display.update()

# SCENE MANAGEMENT (dynamic scene elements)
def idle_dynamic():
    # Clear the animation area by filling it with black
    screen.fill((0, 0, 0), animation_rect)

    # Draw the current frame of animation
    screen.blit(frames[frame_index], (frame_x, frame_y))

    # Update only the animated area (animation rectangle)
    pygame.display.update(animation_rect)

    # Move to the next frame
    frame_index = (frame_index + 1) % len(frames)
    
# PRELOADING AND RENDERING: GENERAL DISPLAY
"""Set up the display"""
screen = pygame.display.set_mode((screen_width,screen_height), pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED, vsync=1)
pygame.display.set_caption("PunchPal")

# PRELOADING AND RENDERING: SERIAL COMMUNICATION
"""Start the serial connection thread"""
serial_thread = threading.Thread(target=connect_serial)
serial_thread.daemon = True
serial_thread.start()

# PRELOADING AND RENDERING: ANIMATIONS
"""Load the spritesheet frames"""
frames = load_spritesheet(spritesheet_path)
frame_index = 0

# PRELOADING AND RENDERING: CLOCK, INIT SCREEN & LOOP
"""initialize clock"""
clock = pygame.time.Clock()

"""load first scene"""
booting()

# MAIN LOOP
running = True
current_state = scene

while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False

    # Read from the serial port 
    # & trigger static scene management
    readSerial()
    
    # Dynamic scene management
    if scene == "BOOTING": # the booting screen is static
        True 
    
    elif scene == "IDLE":
        idle_dynamic()
        
    elif scene == "CHALLENGE1":
        # main title
        text = font_H1.render("Challenge 1", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CHALLENGE1_DEBRIEF":
        # main title
        text = font_H1.render("Challenge 1 Debrief", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CHALLENGE2":
        # main title
        text = font_H1.render("Challenge 2", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CHALLENGE2_DEBRIEF":
        # main title
        text = font_H1.render("Challenge 2 Debrief", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CHALLENGE3":
        # main title
        text = font_H1.render("Challenge 3", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CHALLENGE3_DEBRIEF":
        # main title
        text = font_H1.render("Challenge 3 Debrief", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()
    
    elif scene == "CONCLUSION":
        # main title
        text = font_H1.render("Conclusion", True, (255, 255, 255))
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

