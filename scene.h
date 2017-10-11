#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <limits>
#include <memory>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "util.h"
#include "geometry.h"
#include "linalg/linalg.h"
#include "sphere.h"


class Scene {
private:
    std::vector<std::unique_ptr<Geometry>> objects;
    std::vector<vec> lights;
    vec cameraPos;
    vec screenPos;

    int width;
    int height;


    const std::unique_ptr<Geometry>& getClosestGeom(Ray ray) const;
    double smallestDistanceToObjectInRay(Ray ray) const;

public:

    const Ray getRay(int x, int y);
    std::vector<vec> lightsHit(const vec& point);
    Color traceRay(Ray ray, int depth);

    Scene(vec cameraPos, vec screenPos, int width, int height):
        cameraPos(cameraPos), screenPos(screenPos), width(width), height(height) {}

    Scene(int width, int height):
        Scene(vec(0.0f,0.0f,0.0f), vec(0.0f, 0.0f, 1.0f), width, height) {}

    void addLight(vec light) {
        lights.push_back(light);
    }

    void addToScene(std::unique_ptr<Geometry> g) {
        objects.push_back(std::move(g));
    }
};

#endif
