# This script demonstrates how to extract frames from 
# a spritesheet and animate them using Pygame. 
# The spritesheet is assumed to be a single image containing multiple frames 
# arranged in a grid. The script extracts each frame from the spritesheet 
# and stores them in a list. The animation loop then displays each frame 
# in sequence, creating the illusion of movement.

# libraries
import pygame
from pathlib import Path

# define spritesheet metadata
COL_COUNT = 14  # the spritesheet has 14 columns
ROW_COUNT  = 9  # the spritesheet has 9 rows
SPRITE_WIDTH = 195     # each frame is 195 pixels wide
SPRITE_HEIGHT  = 300    # each frame is 300 pixels high
VALID_FRAMES = (COL_COUNT*ROW_COUNT)-5 # the last 5 frames in the spriteheet are blank

# manage paths
script_dir = Path(__file__).parent
data_dir = script_dir.parent / 'data'
spritesheet_path = data_dir / 'boxer_spritesheet.png'

# initialize Pygame
pygame.init()

# get the screen size
screen_info = pygame.display.Info()
SCREEN_WIDTH = screen_info.current_w
SCREEN_HEIGHT = screen_info.current_h

# set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH ,SCREEN_HEIGHT ), 
    pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED, 
    vsync=1
    )
pygame.display.set_caption("Boxer Animation")

# load the spritesheet
spritesheet = pygame.image.load(spritesheet_path).convert_alpha()

# Preload and scale frames
frames = []
for row in range(ROW_COUNT):
    for col in range(COL_COUNT):
        frame_index = row * COL_COUNT + col
        if frame_index >= VALID_FRAMES:  # Stop if we've reached the total valid frames
            break
        # Extract the subimage (frame) from the spritesheet
        frame = spritesheet.subsurface(pygame.Rect(col * SPRITE_WIDTH, row * SPRITE_HEIGHT , SPRITE_WIDTH, SPRITE_HEIGHT ))
        frames.append(frame)

# Center position for frames
frame_x = (SCREEN_WIDTH - SPRITE_WIDTH) // 2
frame_y = (SCREEN_HEIGHT - SPRITE_HEIGHT) // 2

# Animation loop
frame_index = 0
clock = pygame.time.Clock()
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False

    # Clear the screen before drawing the new frame
    screen.fill((0, 0, 0)) 

    # Display the current frame in the center of the screen
    screen.blit(frames[frame_index], (frame_x, frame_y))

    # Update the display
    pygame.display.flip()

    # Move to the next frame
    frame_index = (frame_index + 1) % len(frames)

    # Limit the frame rate
    clock.tick(24)  # 24 FPS (adjust as needed)
    
pygame.quit()
