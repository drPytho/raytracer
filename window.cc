#include "window.h"



Window::Window(const int width, const int height, const std::string& title)
    : width(width), height(height), title(title) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initiate video. \n";
        std::exit(-1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, width, height,
                              SDL_WINDOW_SHOWN);

    if (window == nullptr) {
            std::cerr << "Could not create a window. \n";
            std::exit(-1);
    }

    //Get window surface
    this->screen = SDL_GetWindowSurface(window);

    //Fill the surface white
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00, 0xCC, 0xFF));

    //Update the surface
    SDL_UpdateWindowSurface(window);
}


Window::~Window() {
    SDL_DestroyWindow( window );
    //Quit SDL subsystems
    SDL_Quit();
}

void Window::drawRectToScreen(Rect r, Color c) {
    SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, c.red, c.green, c.blue));
    SDL_UpdateWindowSurface(window);
}
