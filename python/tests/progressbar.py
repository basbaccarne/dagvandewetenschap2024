# This script runs a progress bar at the bottom of the screen.

import pygame
import time
from pathlib import Path

# Initialize Pygame
pygame.init()

# FPS
FPS = 24

# PATHS
SCRIPT_DIR = Path(__file__).parent
DATA_DIR = SCRIPT_DIR.parent / 'data'
FONT_PATH = DATA_DIR / 'JosefinSans-VariableFont_wght.ttf'

# DESIGN
BRIGHT_YELLOW = (255, 230, 0)
BG_COLOR = (34, 34, 34)
FONT = pygame.font.Font(FONT_PATH, 100)

# SCREEN SIZE
screen_info = pygame.display.Info()
SCREEN_WIDTH = screen_info.current_w
SCREEN_HEIGHT = screen_info.current_h

# GLOBAL VARIABLES
bar_height = SCREEN_HEIGHT // 20
bar_width = SCREEN_WIDTH
bar_area = pygame.Rect(0, SCREEN_HEIGHT - bar_height, bar_width, bar_height)
bin_width = 0
countdown = 0
start_time = 0
last_update_time = 0


def init_Progress_bar(time_duration):
    """Initialize the progress bar with the given duration."""
    global bin_width, countdown, start_time

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
        text = FONT.render(f"  {str(round(remaining_time))}  ", True, (BRIGHT_YELLOW))
        text_rect = text.get_rect()
        text_rect.center = (SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2)

        # Clear the text area and render the new text
        screen.fill(BG_COLOR, text_rect)
        screen.blit(text, text_rect)

        pygame.display.update(progress_rect, text_rect)

# Set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT),
                                  pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED,
                                  vsync=1)
pygame.display.set_caption("Progress Bar")

# Precalculations & preload
init_Progress_bar(10)

# Animation loop
clock = pygame.time.Clock()
running = True
while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False

    # Update the progress bar
    update_progress_bar()

    # Limit the frame rate
    clock.tick(FPS)

pygame.quit()
