import pygame
import random
from pathlib import Path

# Initialize Pygame
pygame.init()

# FPS and Constants
FPS = 24
BRIGHT_YELLOW = (255, 230, 0)
BG_COLOR = (34, 34, 34)
MEDIUM_GRAY = (138, 138, 138)
BAR_HEIGHT = 200

# PATHS
SCRIPT_DIR = Path(__file__).parent
DATA_DIR = SCRIPT_DIR.parent / 'data'
FONT_PATH = DATA_DIR / 'JosefinSans-VariableFont_wght.ttf'
FONT = pygame.font.Font(FONT_PATH, 100)

# Screen setup
screen_info = pygame.display.Info()
SCREEN_WIDTH = screen_info.current_w
SCREEN_HEIGHT = screen_info.current_h

# Constants
DECREASE_SPEED = 10

# Initial punch bar width & speed of decrease
bar_width = 0
max_value = 0
previous_bar_width = 0

def update_punchbar():
    """Sets the bar width to a random punch value."""
    global bar_width, max_value

    # Clear the previous bar
    clear_rect = pygame.Rect(0, SCREEN_HEIGHT // 2 - BAR_HEIGHT // 2, bar_width, BAR_HEIGHT)
    pygame.draw.rect(screen, MEDIUM_GRAY, clear_rect)

    # Set the new punch value and draw the bar
    punch_value = random.randint(100, SCREEN_WIDTH // 2)
    print(f"Punch value: {punch_value}")
    bar_width = punch_value

    # Update the max value if needed
    if punch_value > max_value:
        max_value = punch_value

        # Draw the max value
        text_x =  max_value
        text_y = SCREEN_HEIGHT // 2
        text = FONT.render(str(max_value), True, BRIGHT_YELLOW)
        text_rect = text.get_rect(midleft=(text_x, text_y))
        text_rect.inflate_ip(200, 50)
        text_rect.midleft = (text_x, text_y)
        pygame.draw.rect(screen, BG_COLOR, text_rect)
        screen.blit(text, text.get_rect(center=text_rect.center))
        pygame.display.update(text_rect)

    # Draw the new bar
    punchbar_rect = pygame.Rect(0, SCREEN_HEIGHT // 2 - BAR_HEIGHT // 2, bar_width, BAR_HEIGHT)
    pygame.draw.rect(screen, BRIGHT_YELLOW, punchbar_rect)


def draw_bar_update(previous_width, current_width):
    """Efficiently update the punch bar by redrawing only changed parts."""

    # Calculate the y position of the bar
    y_position = SCREEN_HEIGHT // 2 - BAR_HEIGHT // 2

    # Draw the extended bar or clear when shrinking
    if current_width > previous_width:
        punchbar_rect = pygame.Rect(previous_width, y_position, current_width - previous_width, BAR_HEIGHT)
        pygame.draw.rect(screen, BRIGHT_YELLOW, punchbar_rect)
    else:
        clear_rect = pygame.Rect(current_width, y_position, previous_width - current_width, BAR_HEIGHT)
        pygame.draw.rect(screen, MEDIUM_GRAY, clear_rect)

    # Update only the affected part
    pygame.display.update()

# Set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT),
                                  pygame.FULLSCREEN | pygame.DOUBLEBUF | pygame.HWSURFACE | pygame.SCALED,
                                  vsync=1)
pygame.display.set_caption("Punch Bar")


# initialize backgorund
screen.fill(BG_COLOR)
pygame.display.update()


# Animation loop
clock = pygame.time.Clock()
running = True

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
        bar_width = max(0, bar_width - DECREASE_SPEED)

    # Update the bar only if there's a change
    if bar_width != previous_bar_width:
        draw_bar_update(previous_bar_width, bar_width)

    # Limit the frame rate to 24 FPS
    clock.tick(FPS)

pygame.quit()
