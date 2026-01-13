#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <cmath>

#define START_RADIUS 20
#define TARGET_FPS 60
#define COLOR_RECT_SIZE 10

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 600;

/* 
    Draw Mini Color Palette to choose from
*/
void draw_palette(SDL_Surface* surface, Uint32* colors, int size)
{
    SDL_Rect color_rect;
    for (int i = 0; i < size; ++i) {
        color_rect = {i*COLOR_RECT_SIZE, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE};
        SDL_FillRect(surface, &color_rect, colors[i]);
    }
}

/*
    Draws a cicle at center coordinates with given radius and color
*/
void draw_circle(SDL_Surface* surface, int x_center, int y_center, int radius, Uint32 color) 
{
    SDL_Rect pixel = {0, 0, 1, 1};
    for (int x = x_center-radius; x < x_center + radius; ++x) {
        for (int y = y_center-radius; y < y_center + radius; ++y) {
            /* is the pixel part of the cirlce?? */
            int distance_from_center = std::sqrt(std::pow(x - x_center, 2) + std::pow(y - y_center, 2)); 
            if (distance_from_center < radius) {
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(surface, &pixel, 0x00FF0000);
            }
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Mega Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    bool running = true;
    bool drawing = false;
    SDL_Event event;

    int x;
    int y;
    int radius = START_RADIUS;
    Uint32 color_palette[] = {0x000000, 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF};

    float delay_ms = (1.0f / TARGET_FPS) * 1000;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x;
                    y = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    drawing = true;
                    x = event.motion.x;
                    y = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONUP: 
                    drawing = false;
                    break;
            }
        }

        if (drawing) {
            draw_circle(surface, x, y, radius, 0x00FF0000);
            draw_palette(surface, color_palette, sizeof(color_palette));
            SDL_UpdateWindowSurface(window);
        }

        SDL_Delay(delay_ms);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
