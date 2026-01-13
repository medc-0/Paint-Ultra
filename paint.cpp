#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 600;

constexpr int START_RADIUS = 20;
constexpr int TARGET_FPS = 60;

void draw_circle(SDL_Surface* surface, int x_center, int y_center, int radius, Uint32 color) 
{
    SDL_Rect pixel = {0, 0, 1, 1};
    for (int x = x_center-radius; x < x_center + radius; ++x) {
        for (int y = y_center-radius; y < y_center + radius; ++y) {
            /* is the pixel part of the cirlce?? */
            int distance_from_center = std::sqrt(std::pow( x - x_center, 2) + std::pow(y - y_center, 2)); 
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

    float delay_ms = (1.0f / TARGET_FPS) * 1000;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEMOTION:
                    drawing = true;
                    x = event.motion.x;
                    y = event.motion.y;
                    break;
            }
        }

        if (drawing) {
            draw_circle(surface, x, y, radius, 0x00FF0000);
            SDL_Rect rect = {x, y, 20, 20};
            SDL_FillRect(surface, &rect, 0x00FF0000);
            SDL_UpdateWindowSurface(window);
        }

        SDL_Delay(delay_ms);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
