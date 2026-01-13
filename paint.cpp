#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 600;

constexpr int TARGET_FPS = 60;

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Mega Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Rect rect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 200, 200};
    SDL_FillRect(surface, &rect, 0x00FF0000);
    SDL_UpdateWindowSurface(window);

    bool running = true;
    SDL_Event event;

    float delay_ms = (1.0f / TARGET_FPS) * 1000;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(delay_ms);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
