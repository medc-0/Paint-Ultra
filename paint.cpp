#define SDL_MAIN_HANDLED
#include "paint.h"

PaintApp::PaintApp()
    : color(0xFF000000) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Ultra Pain(t)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    screen = SDL_GetWindowSurface(window);

    canvas = SDL_CreateRGBSurfaceWithFormat(0, CANVAS_W, CANVAS_H, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_FillRect(canvas, nullptr, 0xFFFFFFFF);
}

PaintApp::~PaintApp() {
    if (canvas) SDL_FreeSurface(canvas);
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
    // SDL_Rect fillpixel = {0, PALETTE_HEIGHT, WIDTH, HEIGHT - PALETTE_HEIGHT};
    // SDL_FillRect(canvas, &fillpixel, resetcolor);
    SDL_FillRect(canvas, nullptr, resetcolor);
    // SDL_UpdateWindowSurface(window);
}

/*  
    Fill the canvas whole canvas with the current selected color
*/
void PaintApp::fill_canvas()
{
    // std::cout << "key:F\n";
    // SDL_Rect fpixel = {0, 20, WIDTH, HEIGHT - PALETTE_HEIGHT};
    SDL_FillRect(canvas, nullptr, color);
    // draw_palette();
    // SDL_UpdateWindowSurface(window);
}

/*
    Draws the color palette UI at the top of the window.
*/
void PaintApp::draw_palette()
{
    SDL_Rect r; 
    for (int i = 0; i < COLOR_PALETTE_SIZE; ++i) {
        r = {i * COLOR_RECT_SIZE, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE};
        SDL_FillRect(screen, &r, color_palette[i]);
    }
}

/*
    Draws a filled circle using squared distance checks
*/

void PaintApp::draw_circle(int cx, int cy, int radius, Uint32 col)
{
    int r2 = radius * radius;
    SDL_Rect pixel{0, 0, 1, 1};

    for (int x = cx - radius; x <= cx + radius; ++x) {
        for (int y = cy - radius; y <= cy + radius; ++y) {

            if (x < 0 || y < 0 || x >= canvas->w || y >= canvas->h)
                continue;

            int dx = x - cx;
            int dy = y - cy;

            if (dx * dx + dy * dy <= r2) {
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect(canvas, &pixel, col);
            }
        }
    }
}

void PaintApp::render() 
{
    SDL_Rect src = { cam_x, cam_y, WIDTH, HEIGHT };
    SDL_Rect dst = { 0, PALETTE_HEIGHT, WIDTH, HEIGHT - PALETTE_HEIGHT };

    SDL_BlitSurface(canvas, &src, screen, &dst);
    draw_palette();
    SDL_UpdateWindowSurface(window);
}

void PaintApp::run()
{
    bool running = true;
    bool drawing = false;

    SDL_Event event;
    int mouse_x = 0, mouse_y = 0;
    int brush_size = START_RADIUS;

    Uint32 frame_delay = 1000 / TARGET_FPS;

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
                if (inside_color_palette(mouse_x, mouse_y)) {
                    pick_color(mouse_x);
                } else {
                    drawing = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                drawing = false;
                break;

            case SDL_MOUSEWHEEL:
                brush_size += (event.wheel.y > 0) ? 1 : -1;
                brush_size = std::max(1, brush_size);
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
                if (event.key.keysym.sym == SDLK_c) clear_canvas(0xFFFFFFFF);
                if (event.key.keysym.sym == SDLK_f) fill_canvas();
                if (event.key.keysym.sym == SDLK_p) IMG_SavePNG(canvas, "saved.png");

                if (event.key.keysym.sym == SDLK_a) cam_x -= 40;
                if (event.key.keysym.sym == SDLK_d) cam_x += 40;
                if (event.key.keysym.sym == SDLK_w) cam_y -= 40;
                if (event.key.keysym.sym == SDLK_s) cam_y += 40;

                cam_x = std::clamp(cam_x, 0, CANVAS_W - WIDTH);
                cam_y = std::clamp(cam_y, 0, CANVAS_H - HEIGHT);
                break;
            }
        }

        if (drawing && mouse_y >= PALETTE_HEIGHT) {
            int world_x = mouse_x + cam_x;
            int world_y = mouse_y + cam_y - PALETTE_HEIGHT;
            draw_circle(world_x, world_y, brush_size, color);
        }

        render();
        SDL_Delay(frame_delay);
    }
}

int main()
{
    PaintApp app;
    app.run();
    return 0;
}