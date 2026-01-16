#define SDL_MAIN_HANDLED
#include "paint.h"

#define START_COLOR 0xFF0000

static const int WIDTH = 900;
static const int HEIGHT = 600;

PaintApp::PaintApp()
    : color(START_COLOR), window(nullptr), surface(nullptr) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Ultra Pain(t)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    surface = SDL_GetWindowSurface(window);
}

PaintApp::~PaintApp() {
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
/*
    Returns true if (x, y) is inside the color palette area.
*/
bool PaintApp::inside_color_palette(int x, int y)
{
    return x >= 0 && x < COLOR_PALETTE_SIZE * COLOR_RECT_SIZE && y >= 0 && y < PALETTE_HEIGHT;
}

/*
    Updates the current color based on mouse position.
    Assumes the mouse is already inside the palette.
*/
void PaintApp::pick_color(int x)
{
    int index = x / COLOR_RECT_SIZE;
    color = color_palette[index];
}

/*
    Clear the screen with a default chosen color when the key C is pressed
*/
void PaintApp::clear_canvas(Uint32 resetcolor) 
{
    // std::cout << "key:C\n";
    SDL_Rect fillpixel = {0, PALETTE_HEIGHT, WIDTH, HEIGHT - PALETTE_HEIGHT};
    SDL_FillRect(surface, &fillpixel, resetcolor);
    // draw_palette();
    SDL_UpdateWindowSurface(window);
}

/*  
    Fill the canvas whole canvas with the current selected color
*/
void PaintApp::fill_canvas()
{
    // std::cout << "key:F\n";
    SDL_Rect fpixel = {0, 20, WIDTH, HEIGHT - PALETTE_HEIGHT};
    SDL_FillRect(surface, &fpixel, color);
    draw_palette();
    SDL_UpdateWindowSurface(window);
}

/*
    Draws the color palette UI at the top of the window.
*/
void PaintApp::draw_palette()
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
void PaintApp::draw_circle(int cx, int cy, int radius, Uint32 col)
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

void PaintApp::run() {

    bool running = true;
    bool drawing = false;
    bool needs_redraw = true;

    SDL_Event event;

    int mouse_x = 0;
    int mouse_y = 0;
    int brush_size = START_RADIUS;

    Uint32 frame_delay = 1000 / TARGET_FPS;
    draw_palette();
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

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                    if (event.key.keysym.sym == SDLK_c) clear_canvas(0x00000000);
                    if (event.key.keysym.sym == SDLK_f) fill_canvas();
                    break;

                case SDL_MOUSEWHEEL:
                    brush_size += (event.wheel.y > 0) ? 1 : -1;
                    if (brush_size < 1) brush_size = 1;
                    std::cout << "Brush size: " << brush_size << '\n';
                    break;
            }
        }

        if (drawing && mouse_y >= PALETTE_HEIGHT) {
            draw_circle(mouse_x, mouse_y, brush_size, color);
            needs_redraw = true;
        }

        if (needs_redraw) {
            draw_palette();
            SDL_UpdateWindowSurface(window);
            needs_redraw = false;
        }

        SDL_Delay(frame_delay);
    }
}


int main()
{
    PaintApp app;
    app.run();
    return EXIT_SUCCESS;
}
