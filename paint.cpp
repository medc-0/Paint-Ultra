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

    bool running = true;
    bool drawing = false;
    SDL_Event event;

    int x;
    int y;

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
            SDL_Rect rect = {x, y, 50, 50};
            SDL_FillRect(surface, &rect, 0x00FF0000);
            SDL_UpdateWindowSurface(window);
        }

        SDL_Delay(delay_ms);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
