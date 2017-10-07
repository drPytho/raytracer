#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "color.h"

using Rect = SDL_Rect;

class Window {
private:
    SDL_Window* window;
    SDL_Surface* screen;

    const int width;
    const int height;
    const std::string title;
public:

    Window(const int width, const int height, const std::string& title);
    ~Window();
    void drawRectToScreen(Rect r, Color c);
};

#endif
