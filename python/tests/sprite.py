# This script demonstrates how to extract frames from 
# a spritesheet and animate them using Pygame. 
# The spritesheet is assumed to be a single image containing multiple frames 
# arranged in a grid. The script extracts each frame from the spritesheet 
# and stores them in a list. The animation loop then displays each frame 
# in sequence, creating the illusion of movement.

# libraries
import pygame
from pathlib import Path

# initialize Pygame
pygame.init()

# define spritesheet metadata
COL_COUNT = 14  # the spritesheet has 14 columns
ROW_COUNT  = 9  # the spritesheet has 9 rows
FPS = 24        # frames per second
SPRITE_WIDTH = 195     # each frame is 195 pixels wide
SPRITE_HEIGHT  = 300    # each frame is 300 pixels high
VALID_FRAMES = (COL_COUNT*ROW_COUNT)-5 # the last 5 frames in the spriteheet are blank
TITLE_FONT_SIZE = 48  # Font size for the title

# manage paths
script_dir = Path(__file__).parent
data_dir = script_dir.parent / 'data'
spritesheet_path = data_dir / 'boxer_spritesheet.png'

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

def draw_static_elements(screen, screen_width, screen_height, title):
    """Draw static elements (e.g., title) that don't change every frame."""
    screen.fill((0, 0, 0))
    title_surface = title.render("Boxer Animation Demo", True, (255, 255, 255)) 
    title_x = (screen_width - title_surface.get_width()) // 2
    screen.blit(title_surface, (title_x, 20)) 

# get the screen size
screen_info = pygame.display.Info()
SCREEN_WIDTH = screen_info.current_w
SCREEN_HEIGHT = screen_info.current_h

# set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH ,SCREEN_HEIGHT), 
    pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED, 
    vsync=1
    )
pygame.display.set_caption("Boxer Animation")

# Load the spritesheet frames
frames = load_spritesheet(spritesheet_path)

# Center position for the animated sprite
frame_x = (SCREEN_WIDTH - SPRITE_WIDTH) // 2
frame_y = (SCREEN_HEIGHT - SPRITE_HEIGHT) // 2 + 100
animation_rect = pygame.Rect(frame_x, frame_y, SPRITE_WIDTH, SPRITE_HEIGHT)


# Set up static title rendering
title_font = pygame.font.Font(None, TITLE_FONT_SIZE)
draw_static_elements(screen, SCREEN_WIDTH, SCREEN_HEIGHT, title_font)  # Draw once

# Update the static elements on screen
pygame.display.update()

# Animation loop
frame_index = 0
clock = pygame.time.Clock()
running = True

while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False
    
    # Clear the animation area by filling it with black
    screen.fill((0, 0, 0), animation_rect)

    # Draw the current frame of animation
    screen.blit(frames[frame_index], (frame_x, frame_y))

    # Update only the animated area (animation rectangle)
    pygame.display.update(animation_rect)

    # Move to the next frame
    frame_index = (frame_index + 1) % len(frames)

    # Limit the frame rate
    clock.tick(FPS)
    
pygame.quit()
