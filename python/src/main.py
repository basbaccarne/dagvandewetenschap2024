# This is the main script for the PunchPal project.
# It is responsible for the following:
# - Initializing the Pygame environment
# - Setting up the serial connection
# - Parsing the incoming serial data
# - Reading from the serial port and setting variables
# - Managing the different scenes
# - Rendering the static scene elements
# - Rendering the dynamic scene elements

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
SCREEN_INFO = pygame.display.Info()
SCREEN_WIDTH = SCREEN_INFO.current_w
SCREEN_HEIGHT = SCREEN_INFO.current_h

pygame.mouse.set_visible(False)

"""paths"""
SCRIPT_DIR = Path(__file__).parent
DATA_DIR = SCRIPT_DIR.parent / 'data'
SPRITESHEET_PATH = DATA_DIR / 'boxer_spritesheet.png'
FONT1_PATH = DATA_DIR / 'Jost-VariableFont_wght.ttf'
FONT2_PATH = DATA_DIR / 'JosefinSans-VariableFont_wght.ttf'

"""design system"""
BG_COLOR = (34, 34, 34)
GRAY = (82, 82, 82)
MEDIUM_GRAY = (138, 138, 138)
LIGHTGRAY = (216, 216, 216)
BRIGHT_YELLOW = (255, 230, 0)

FONT1 = pygame.font.Font(FONT1_PATH, 100)
FONT2 = pygame.font.Font(FONT2_PATH, 50)
FONT3 = pygame.font.Font(FONT2_PATH, 100)

"""animated sprite settings"""
COL_COUNT = 14  # the spritesheet has 14 columns
ROW_COUNT  = 9  # the spritesheet has 9 rows
FPS = 24        # frames per second
SPRITE_WIDTH = 195     # each frame is 195 pixels wide
SPRITE_HEIGHT = 300    # each frame is 300 pixels high
VALID_FRAMES = (COL_COUNT * ROW_COUNT) - 5 # the last 5 frames in the spriteheet are blank


"""progress bar variables"""
bar_height = SCREEN_HEIGHT // 20
bar_width = SCREEN_WIDTH
bar_area = pygame.Rect(0, SCREEN_HEIGHT - bar_height, bar_width, bar_height)
bin_width = 0
countdown = 0
start_time = 0
last_update_time = 0


# FUNCTIONS
def load_spritesheet(SPRITESHEET_PATH):
    """Function to load a spritesheet"""
    global COL_COUNT, ROW_COUNT, SPRITE_WIDTH, SPRITE_HEIGHT, VALID_FRAMES
    
    """Load and preprocess frames from the spritesheet."""
    spritesheet = pygame.image.load(SPRITESHEET_PATH).convert_alpha()
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


ser = None
def connect_serial():
    """Function to set-up the serial connection"""
    global ser

    while True:
        try:
            ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1) 
            print("Serial connection established")
            break
        except serial.SerialException:
            print("Failed to connect to serial port. Retrying...")
            time.sleep(2)


serial_dict = {}
def parseSerial(line):
    """Function to parse the incoming serial data"""
    global serial_dict

    # Parse the incoming data from 'message||value' to dictionary
    if "||" in line:
        message, value = line.split("||")
        serial_dict[message] = value
        # print(f"Parsed: {serial_dict}")


def readSerial():
    """Function to read from the serial port and set variables"""
    global ser, current_state, scene

    try:
        if ser and ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            # uncomment to print raw serial input
            print(line) 

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
        print(f"Nothing to parse || error: {e}")
        ser = None
        # Restart the serial connection thread
        serial_thread = threading.Thread(target=connect_serial)
        serial_thread.daemon = True
        serial_thread.start()

def init_Progress_bar(time_duration):
    """Initialize the progress bar with the given duration."""
    global bin_width, countdown, last_update_time

    # Bar color
    bar_color = BRIGHT_YELLOW

    # Render the background
    screen.fill(BG_COLOR)

    # Draw the initial progress bar
    pygame.draw.rect(screen, bar_color, bar_area)
    pygame.display.update(bar_area)
    
    # Calculate bin width and set the countdown time
    bin_width = bar_width / time_duration
    countdown = time_duration
    start_time = time.time()
    last_update_time = 0


def update_progress_bar():
    """Update the progress bar based on elapsed time."""
    global last_update_time

    elapsed_time = time.time() - start_time
    remaining_time = countdown - elapsed_time

    # Update the progress bar every second
    if remaining_time > -1 and elapsed_time - last_update_time >= 1:
        last_update_time = elapsed_time

        # subtract the bin width from the right side of the bar
        retract_x = SCREEN_WIDTH - (bin_width * elapsed_time)
        progress_rect = pygame.Rect(retract_x, bar_area.top, bin_width + 10, bar_height)    
        pygame.draw.rect(screen, BG_COLOR, progress_rect)

        # Render the remaining time text
        text = FONT3.render(f"  {str(round(remaining_time))}  ", True, (BRIGHT_YELLOW))
        text_rect = text.get_rect()
        text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2)

        # Clear the text area and render the new text
        screen.fill(BG_COLOR, text_rect)
        screen.blit(text, text_rect)

        pygame.display.update(progress_rect, text_rect)
    

# SCENE MANAGEMENT (static scene elements)
"""Set initial scene"""
scene = "BOOTING"

"""Function to render the booting screen (static elements)"""
def booting():
    # background
    screen.fill(BG_COLOR)
    # main title
    text = FONT1.render("Syteem operationeel!", True, (MEDIUM_GRAY))
    text_rect = text.get_rect()
    text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2)
    screen.blit(text, text_rect)
    # subtitle
    text = FONT2.render("Klop op de zak om verder te gaan", True, (BRIGHT_YELLOW))
    text_rect = text.get_rect()
    text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2 + 100)
    screen.blit(text, text_rect)
    # render
    pygame.display.flip()

"""Function to render the idle screen (static elements)"""
def idle():
    global animation_rect, frame_x, frame_y, frames
    # background
    screen.fill(BG_COLOR)
    # main title
    text = FONT1.render("Klop om de zak om te beginnen.", True, (MEDIUM_GRAY))
    text_rect = text.get_rect()
    text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 4)
    screen.blit(text, text_rect)
    # render
    pygame.display.flip()
    # Define position for the animated sprite (only once)
    frame_x = (SCREEN_WIDTH - SPRITE_WIDTH) // 2
    frame_y = (SCREEN_HEIGHT - SPRITE_HEIGHT) // 2 + 100
    animation_rect = pygame.Rect(frame_x, frame_y, SPRITE_WIDTH, SPRITE_HEIGHT)

"""Function to render the challenge 1 screen (static elements)"""
def challenge1():
    # background
    screen.fill(BG_COLOR)
    # main title
    text = FONT1.render("Uitdaging 1: Kracht", True, (MEDIUM_GRAY))
    text_rect = text.get_rect()
    text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 6)
    screen.blit(text, text_rect)
    # subtitle
    text = FONT2.render("Sla zo hard als je kan!", True, (BRIGHT_YELLOW))
    text_rect = text.get_rect()
    text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2 + 100)
    screen.blit(text, text_rect)
    # progress bar
    init_Progress_bar(15)
    # render
    pygame.display.flip()

""""Function to render the challenge 1 debrief screen"""
def challenge1_debrief():
    screen.fill(LIGHTGRAY)
    pygame.display.update()
    # progress bar
    init_Progress_bar(12)

"""Function to render the challenge 2 screen"""
def challenge2():
    screen.fill(BRIGHT_YELLOW)
    pygame.display.update()
    # progress bar
    init_Progress_bar(30)

"""Function to render the challenge 2 debrief screen"""
def challenge2_debrief():
    screen.fill(BG_COLOR)
    pygame.display.update()
    # progress bar
    init_Progress_bar(15)

"""Function to render the challenge 3 screen"""
def challenge3():
    screen.fill(GRAY)
    pygame.display.update()
    # progress bar
    init_Progress_bar(30)

"""Function to render the challenge 3 debrief screen"""
def challenge3_debrief():
    screen.fill(MEDIUM_GRAY)
    pygame.display.update()
    # progress bar
    init_Progress_bar(5)

"""Function to render the conclusion screen"""
def conclusion():
    screen.fill(LIGHTGRAY)
    pygame.display.update()
    # progress bar
    init_Progress_bar(20)

# SCENE MANAGEMENT (dynamic scene elements)
"""Function to render the idle screen (animated boxer)"""
def idle_dynamic():
    global frame_index
    screen.fill((BG_COLOR), animation_rect)
    screen.blit(frames[frame_index], (frame_x, frame_y))
    pygame.display.update(animation_rect)
    frame_index = (frame_index + 1) % len(frames)
    
# PRELOADING AND RENDERING: GENERAL DISPLAY
"""Set up the display"""
screen = pygame.display.set_mode((SCREEN_WIDTH,SCREEN_HEIGHT), pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED, vsync=1)
pygame.display.set_caption("PunchPal")

# PRELOADING AND RENDERING: SERIAL COMMUNICATION
"""Start the serial connection thread"""
serial_thread = threading.Thread(target=connect_serial)
serial_thread.daemon = True
serial_thread.start()

# PRELOADING AND RENDERING: ANIMATIONS
"""Load the spritesheet frames"""
frames = load_spritesheet(SPRITESHEET_PATH)
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
        update_progress_bar()
    
    elif scene == "CHALLENGE1_DEBRIEF":
        # main title
        text = FONT1.render("Challenge 1 Debrief", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()

        update_progress_bar()
    
    elif scene == "CHALLENGE2":
        # main title
        text = FONT1.render("Challenge 2", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()

        update_progress_bar()
    
    elif scene == "CHALLENGE2_DEBRIEF":
        # main title
        text = FONT1.render("Challenge 2 Debrief", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()

        update_progress_bar()
    
    elif scene == "CHALLENGE3":
        # main title
        text = FONT1.render("Challenge 3", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()

        update_progress_bar()
    
    elif scene == "CHALLENGE3_DEBRIEF":
        # main title
        text = FONT1.render("Challenge 3 Debrief", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()

        update_progress_bar()
    
    elif scene == "CONCLUSION":
        # main title
        text = FONT1.render("Conclusion", True, (255, 255, 255))
        text_rect = text.get_rect(center=(400, 300))
        screen.blit(text, text_rect)
        pygame.display.flip()

        update_progress_bar()

    # Cap the frame rate
    clock.tick(FRAMERATE)

# Quit Pygame
if ser:
    ser.close()
pygame.quit()
sys.exit()

