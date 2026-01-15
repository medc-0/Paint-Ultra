#ifndef PAINT_H
#define PAINT_H

#include <SDL2/SDL.h>
#include <iostream>

class PaintApp {
    public:
        PaintApp();
        ~PaintApp();
        void run();

        private:
        SDL_Window* window;
        SDL_Surface* surface;
        bool inside_color_palette(int x, int y);
        void pick_color(int x);
        void draw_palette(SDL_Surface* surface);
        void draw_circle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 col);
        void clear_screen(SDL_Surface* surface, Uint32 fillcolor);
        static const int COLOR_PALETTE_SIZE = 8;
        static const int COLOR_RECT_SIZE = 20;
        static const int PALETTE_HEIGHT = COLOR_RECT_SIZE;
        static const int START_RADIUS = 5;
        static const int TARGET_FPS = 60;
        Uint32 color;
        Uint32 color_palette[COLOR_PALETTE_SIZE] = {0x000000, 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF};
};

#endif