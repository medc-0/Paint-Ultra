#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

#define COLOR_RECT_SIZE 20
#define START_COLOR 0xFF0000

constexpr int SCREEN_WIDTH = 900;
constexpr int SCREEN_HEIGHT = 600;
constexpr int START_RADIUS = 20;
constexpr int TARGET_FPS = 60;
constexpr int COLOR_PALETTE_SIZE = 8;

constexpr int PALETTE_HEIGHT = COLOR_RECT_SIZE;

Uint32 color = START_COLOR;
Uint32 color_palette[COLOR_PALETTE_SIZE] = {0x000000, 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF};

/*
    Returns true if (x, y) is inside the color palette area.
*/
bool inside_color_palette(int x, int y)
{
    return x >= 0 && x < COLOR_PALETTE_SIZE * COLOR_RECT_SIZE && y >= 0 && y < PALETTE_HEIGHT;
}

/*
    Updates the current color based on mouse position.
    Assumes the mouse is already inside the palette.
*/
void pick_color(int x)
{
    int index = x / COLOR_RECT_SIZE;
    color = color_palette[index];
}

/*
    Draws the color palette UI at the top of the window.
*/
void draw_palette(SDL_Surface* surface)
{
    SDL_Rect rect;

    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        rect = { i * COLOR_RECT_SIZE, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE };
        SDL_FillRect(surface, &rect, color_palette[i]);
    }
}

/*
    Draws a filled circle using squared distance checks
*/
void draw_circle(SDL_Surface* surface, int cx, int cy, int radius, Uint32 col)
{
    int r2 = radius * radius;
    SDL_Rect pixel = {0, 0, 1, 1};

    for (int x = cx - radius; x <= cx + radius; ++x) {
        for (int y = cy - radius; y <= cy + radius; ++y) {
            if (x < 0 || y < PALETTE_HEIGHT ||
                x >= surface->w || y >= surface->h)
                continue;

            int dx = x - cx;
            int dy = y - cy;

            if (dx * dx + dy * dy <= r2) {
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(surface, &pixel, col);
            }
        }
    }
}


int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Ultra Pain(t)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    bool running = true;
    bool drawing = false;
    bool needs_redraw = true;

    SDL_Event event;

    int mouse_x = 0;
    int mouse_y = 0;
    int brush_size = START_RADIUS;

    Uint32 frame_delay = 1000 / TARGET_FPS;
    draw_palette(surface);
    SDL_UpdateWindowSurface(window);

    while (running) {

        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEMOTION:
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    mouse_x = event.button.x;
                    mouse_y = event.button.y;

                    if (inside_color_palette(mouse_x, mouse_y)) {
                        pick_color(mouse_x);
                        needs_redraw = true;
                    }
                    else {
                        drawing = true;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    drawing = false;
                    break;

                case SDL_MOUSEWHEEL:
                    brush_size += (event.wheel.y > 0) ? 1 : -1;
                    if (brush_size < 1) brush_size = 1;
                    std::cout << "Brush size: " << brush_size << '\n';
                    break;
            }
        }

        if (drawing && mouse_y >= PALETTE_HEIGHT) {
            draw_circle(surface, mouse_x, mouse_y, brush_size, color);
            needs_redraw = true;
        }

        if (needs_redraw) {
            draw_palette(surface);
            SDL_UpdateWindowSurface(window);
            needs_redraw = false;
        }

        SDL_Delay(frame_delay);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
