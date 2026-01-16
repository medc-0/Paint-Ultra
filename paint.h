#ifndef PAINT_H
#define PAINT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <algorithm>

class PaintApp {
public:
    PaintApp();
    ~PaintApp();
    void run();

private:
    SDL_Window* window = nullptr;
    SDL_Surface* screen = nullptr;

    SDL_Surface* canvas = nullptr;
    int cam_x = 0;
    int cam_y = 0;

    Uint32 color;

    static const int WIDTH = 900;
    static const int HEIGHT = 600;
    static const int CANVAS_W = 8000;
    static const int CANVAS_H = 8000;
    static const int COLOR_PALETTE_SIZE = 8;
    static const int COLOR_RECT_SIZE = 20;
    static const int PALETTE_HEIGHT = COLOR_RECT_SIZE;
    static const int START_RADIUS = 5;
    static const int TARGET_FPS = 60;
    Uint32 color_palette[COLOR_PALETTE_SIZE] = {0xFF000000, 0xFFFFFFFF, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFF00, 0xFF00FFFF, 0xFFFF00FF};
    bool inside_color_palette(int x, int y);
    void pick_color(int x);
    void draw_palette();
    void draw_circle(int cx, int cy, int radius, Uint32 col);
    void fill_canvas();
    void clear_canvas(Uint32 resetcolor);
    void render();
};

#endif
