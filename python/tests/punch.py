import pygame
import random
from pathlib import Path

# Initialize Pygame
pygame.init()

# FPS and Constants
FPS = 24
BRIGHT_YELLOW = (255, 230, 0)
BG_COLOR = (34, 34, 34)
BAR_HEIGHT = 200

# Screen setup
screen_info = pygame.display.Info()
SCREEN_WIDTH = screen_info.current_w
SCREEN_HEIGHT = screen_info.current_h

# Initial punch bar width
bar_width = 0
decrease_speed = 10

def update_punchbar():
    """Sets the bar width to a random punch value."""
    global bar_width

    clear_rect = pygame.Rect(0, SCREEN_HEIGHT // 2 - BAR_HEIGHT // 2, bar_width, BAR_HEIGHT)
    pygame.draw.rect(screen, BG_COLOR, clear_rect)

    punch_value = random.randint(100, SCREEN_WIDTH // 2)
    print(f"Punch value: {punch_value}")
    bar_width = punch_value

    punchbar_rect = pygame.Rect(0, SCREEN_HEIGHT // 2 - BAR_HEIGHT // 2, bar_width, BAR_HEIGHT)
    pygame.draw.rect(screen, BRIGHT_YELLOW, punchbar_rect)



def draw_bar_update(previous_width, current_width):
    """Efficiently update the punch bar by redrawing only changed parts."""
    y_position = SCREEN_HEIGHT // 2 - BAR_HEIGHT // 2

    # Draw the extended bar or clear when shrinking
    if current_width > previous_width:
        punchbar_rect = pygame.Rect(previous_width, y_position, current_width - previous_width, BAR_HEIGHT)
        pygame.draw.rect(screen, BRIGHT_YELLOW, punchbar_rect)
    else:
        clear_rect = pygame.Rect(current_width, y_position, previous_width - current_width, BAR_HEIGHT)
        pygame.draw.rect(screen, BG_COLOR, clear_rect)

    # Update only the affected part
    pygame.display.update()

# Set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT),
                                  pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED,
                                  vsync=1)
pygame.display.set_caption("Punch Bar")

# Animation loop
clock = pygame.time.Clock()
running = True
previous_bar_width = 0

while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False

        # Handle punch (spacebar press)
        if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
            update_punchbar()

    # Gradually decrease the bar width
    if bar_width > 0:
        previous_bar_width = bar_width
        bar_width = max(0, bar_width - decrease_speed)

    

    # Update the bar only if there's a change
    if bar_width != previous_bar_width:
        draw_bar_update(previous_bar_width, bar_width)

    # Limit the frame rate to 24 FPS
    clock.tick(FPS)

pygame.quit()
