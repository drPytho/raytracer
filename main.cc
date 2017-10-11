#include <iostream>
#include <memory>
#include "window.h"
#include "linalg/linalg.h"
#include "scene.h"
#include "sphere.h"
#include "plane.h"

using namespace linalg::aliases;

const int WIDTH = 1920;
const int HEIGHT = 1080;

int main() {
    Window w(WIDTH, HEIGHT, "Raytracer");

    w.drawRectToScreen(Rect{50, 50, 50, 50}, Color{0xFF, 0x00, 0x00});

    Scene s(WIDTH, HEIGHT);
    Geometry *g = new Sphere(Color{0xFF, 0xCC, 0x00}, 0, double3(0.0f, 0.0f, 10.0f), 1.0f);
    g->setOpacity(0.5);
    g->setRefraction(1.51);
//    s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Sphere(Color{0xFF, 0xCC, 0xFF}, 0.5, double3(5.0f, -0.0f, 15.0f), 2.5f);
//    s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Sphere(Color{0x00, 0x00, 0xFF}, 0.4, double3(-2.1f, -1.0f, 10.0f), 1.0f);
    g->setOpacity(0.7);
    g->setRefraction(1.51);
    s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Sphere(Color{0xFF, 0x00, 0x00}, 0.4, double3(-0.0f, -1.0f, 10.0f), 1.0f);
    g->setOpacity(0.7);
    g->setRefraction(1.51);
    s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Sphere(Color{0xFF, 0x00, 0xFF}, 0.2, double3(2.0f, 4.0f, 20.0f), 1.0f);
    g->setOpacity(0.7);
    g->setRefraction(1.01);
    s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Plane(Color{0xFF, 0xFF, 0xFF}, 0, double3(-10.0f, 0.0f, 0.0f), double3(1.0f, 0.0f, -0.1f));
    //s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Plane(Color{0xFF, 0xFF, 0xFF}, 0, double3(10.0f, 0.0f, 0.0f), double3(-1.0f, 0.0f, -0.1f));
    //s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Plane(Color{0xFF, 0xFF, 0xFF}, 0, double3(0.0f, -5.0f, 0.0f), double3(0.0f, 1.0f, -0.1f));
    //s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Plane(Color{0xFF, 0xFF, 0xFF}, 0, double3(0.0f, 5.0f, 0.0f), double3(0.0f, -1.0f, -0.1f));
    //s.addToScene(std::unique_ptr<Geometry>(g));

    g = new Plane(Color{0xDD, 0xDD, 0xDD}, 0, double3(0.0f, 0.0f, 15.0f), double3(0.0f, 0.0f, -1.0f));
    //s.addToScene(std::unique_ptr<Geometry>(g));

    //s.addLight(double3(-10, 10, 0));
    s.addLight(double3(-9, 4.9, 0));

    // Calculate the color with a raytracer.
    for (int x = 0; x < WIDTH; x++) {

        for (int y = 0; y < HEIGHT; y++) {
            Ray toTrace = s.getRay(x, y);
            Color c = s.traceRay(toTrace, 3);
            w.drawRectToScreen(Rect{x, y, 1, 1}, c);
        }
        w.updateWindow();
    }
    w.updateWindow();

    w.saveToFile("tmp.bmp");
    //Wait two seconds
    SDL_Delay(1 * 1000);

     //Destroy window

    return 0;
}
