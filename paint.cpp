#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#define COLOR_RECT_SIZE 20
#define START_COLOR 0xFF0000

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 600;
constexpr int START_RADIUS = 20;
constexpr int TARGET_FPS = 60;
constexpr int COLOR_PALETTE_SIZE = 8;

Uint32 color = START_COLOR;
Uint32 color_palette[] = {0x000000, 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF};

bool inside_color_palette(int x, int y) 
{
    return x <= COLOR_PALETTE_SIZE*COLOR_RECT_SIZE && y <= COLOR_RECT_SIZE;
}

/*
    Check if user clicked color palette and updates color if so
*/
void check_color_palette_choosen(int x, int y) 
{   
    int i;
    if (inside_color_palette(x, y)) {
        /* Mouse is inside x,y range of color palette */
        i = x / COLOR_RECT_SIZE;
        color = color_palette[i];
    }
}

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
            /* is the pixel part of the cirlce */
            int distance_from_center = std::sqrt(std::pow(x - x_center, 2) + std::pow(y - y_center, 2)); 
            if (distance_from_center < radius) {
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ultra Pain(t)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    bool running = true;
    bool drawing = false;
    SDL_Event event;

    int x;
    int y;
    int radius = START_RADIUS;

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
                    check_color_palette_choosen(x, y);
                    break;
                case SDL_MOUSEBUTTONUP: 
                    drawing = false;
                    break;
            }
        }

        if (drawing) {
            draw_circle(surface, x, y, radius, color);
            SDL_UpdateWindowSurface(window);
            SDL_Delay(delay_ms);
        }
        draw_palette(surface, color_palette, COLOR_PALETTE_SIZE);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
