#include <iostream>
#include "window.h"
#include "linalg/linalg.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

int main() {
    Window w(WIDTH, HEIGHT, "Raytracer");

    w.drawRectToScreen(Rect{50, 50, 50, 50}, Color{0xFF, 0x00, 0x00});

    // Calculate the color with a raytracer.



    //Wait two seconds
    SDL_Delay(20000);

     //Destroy window

    return 0;
}
