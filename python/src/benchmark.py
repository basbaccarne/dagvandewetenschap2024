# Load libraries
import pygame
import sys
import os

# Initialize Pygame
pygame.init()

# Constants
FRAMERATE = 400
SCREEN_WIDTH, SCREEN_HEIGHT = 1280,720 
FONT_PATH = "./data/Jost-VariableFont_wght.ttf"
FONT_SIZE = 20
TEXT_COLOR = (255, 230, 0)
BG_COLOR = (34, 34, 34)
MOVE_SPEED = FRAMERATE//500+1

# Set up the display
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), pygame.DOUBLEBUF, 8)
pygame.display.set_caption("Animated Text Demo")

# Hide the mouse cursor
pygame.mouse.set_visible(False)

# Load the font
font = pygame.font.Font(FONT_PATH, FONT_SIZE)

# Pre-render the text surface
text = "Demo text in custom font"
text_surface = font.render(text, True, TEXT_COLOR).convert_alpha()
text_rect = text_surface.get_rect(center=(SCREEN_WIDTH // 2, SCREEN_HEIGHT // 2))
prev_text_rect = text_rect.copy()

# Initialize variables
x, y = text_rect.center
dx, dy = MOVE_SPEED, MOVE_SPEED
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

    # Move the text
    x += dx
    y += dy

    # Bounce the text off the edges
    if x + FONT_SIZE // 2 > SCREEN_WIDTH or x - FONT_SIZE // 2 < 0:
        dx = -dx
    if y + FONT_SIZE // 2 > SCREEN_HEIGHT or y - FONT_SIZE // 2 < 0:
        dy = -dy

    # Clear the old text position (efficency)
    screen.fill(BG_COLOR, text_rect)

    # Render the new text
    text_rect = text_surface.get_rect(center=(x, y))
    screen.blit(text_surface, text_rect)

    # Clear the old framerate text position
    screen.fill(BG_COLOR, (10, 10, 150, 50))

    # Display the new framerate
    framerate_text = font.render(f"FPS: {int(clock.get_fps())}", True, TEXT_COLOR)
    screen.blit(framerate_text, (10, 10))

    # Update the display
    pygame.display.update([prev_text_rect, text_rect, (10, 10, 150, 50)])

    # Save the current text_rect for the next frame
    prev_text_rect = text_rect.copy()

    # Cap the frame rate
    clock.tick(FRAMERATE)

# Quit Pygame
pygame.quit()
sys.exit()