#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <limits>
#include <memory>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "geometry.h"
#include "linalg/linalg.h"
#include "sphere.h"

using namespace linalg::aliases;



const double PI = 3.141592653589793238463;

class Scene {
private:
    std::vector<std::unique_ptr<Geometry>> objects;
    std::vector<double3> lights;
    double3 cameraPos;
    double3 screenPos;

    int width;
    int height;


    const std::unique_ptr<Geometry>& getClosestGeom(Ray ray) const {
        double dist = std::numeric_limits<double>::infinity();
        auto tail = objects.cend();
        for (auto i = objects.cbegin(); i < objects.cend(); i++) {
            const std::unique_ptr<Geometry>& geom = *i;
            if (geom->intersectsWithRay(ray)) {
                double distance = geom->intersectDistance(ray);
                if (distance < dist) {
                    tail = i;
                    dist = distance;
                }
            }
        }
        if (dist == std::numeric_limits<double>::infinity()) {
            return std::move(std::unique_ptr<Geometry>(nullptr));
        } else {
            return *tail;
        }
    }

    double smallestDistanceToObjectInRay(Ray ray) const {
        ray.startPos = ray.startPos + 0.001 * ray.direction;
        auto& ptr = getClosestGeom(ray);
        if (ptr.get() == nullptr) {
            return std::numeric_limits<double>::infinity();
        } else {
            return getClosestGeom(ray)->intersectDistance(ray);
        }
    }
public:

    Scene(double3 cameraPos, double3 screenPos, int width, int height):
        cameraPos(cameraPos), screenPos(screenPos), width(width), height(height) {}

    Scene(int width, int height):
        Scene(double3(0.0f,0.0f,0.0f), double3(0.0f, 0.0f, 1.0f), width, height) {}

    // TODO: Remove magic numbers
    Scene():
        Scene(1280, 720) {}

    void addLight(double3 light) {
        lights.push_back(light);
    }

    const Ray getRay(int x, int y) {

        double newX = (static_cast<double>(x)/static_cast<double>(width) - 0.5f);
        double newY = (-static_cast<double>(y)/static_cast<double>(width) +
                      static_cast<double>(height)/static_cast<double>(width)*0.5f);

        double3 dir{newX, newY, 0.0f};

        return Ray(cameraPos, dir + screenPos);
    }

    void addToScene(std::unique_ptr<Geometry> g) {
        objects.push_back(std::move(g));
    }

    std::vector<double3> lightsHit(const double3& point) {
        std::vector<double3> ret;
        auto pred = [&](const double3& light) {
            // Create a ray from the point to the light
            Ray ray(point, light - point);

            double lightDist2 = linalg::distance2(light, point);
            double objDist = smallestDistanceToObjectInRay(ray);

            bool r = lightDist2 < objDist*objDist;

            return r;
        };

        std::copy_if(lights.begin(), lights.end(),
                     std::back_inserter(ret), pred);
        return ret;
    }

    Color traceRay(Ray ray, int depth) {
        depth--;

        // get geometry closest to ray that intersects with it.
        //std::cout << "running through here \n";
        auto& geom = getClosestGeom(ray);

        if (geom.get() == nullptr) { // nothing caught my eye
            return Color{0xEE, 0xEE, 0xEE};
        }

        auto dist = geom->intersectDistance(ray);
        auto pointOnGeom = ray.startPos + dist*ray.direction;



        Color c = geom->getColor();

        if (geom->getOpacity() != 0 && geom->getType() == "SPHERE") {

            double3 internalDirection = ray.snellsDirection(1, geom->getRefraction(),
                                                           geom->getNormalAt(pointOnGeom));

            Ray internal(pointOnGeom, internalDirection);

            double exitDist = geom->exitDistance(internal);
            double otherSidePower = std::pow(geom->getOpacity(), exitDist);

            double3 otherSidePoint = internal.translate(exitDist+0.001);

            double3 otherSideDir =
                internal.snellsDirection(geom->getRefraction(), 1,
                                         geom->getNormalAt(otherSidePoint));

            if (otherSideDir == double3(0,0,0)) {
                c = Color{0x00, 0xFF, 0x00};
            } else {
                Ray outAgain(otherSidePoint, otherSideDir);
                if (depth+1) {
                    c = traceRay(outAgain, depth) * otherSidePower +
                        geom->getColor() * (1 - otherSidePower);
                } else {
                    c = Color{0xFF, 0, 0xFF};
                }
            }

        }
        auto lightsInPath = lightsHit(pointOnGeom);
        double intencity = 0;

        for (const auto& light : lightsInPath) {
            intencity = linalg::dot(geom->getNormalAt(pointOnGeom),
                                    linalg::normalize(light - pointOnGeom));
        }
        c = c * (intencity + 0.3);




        if (depth && geom->getReflect()) {
            Ray reflection = ray.mirrorIn(pointOnGeom, geom->getNormalAt(pointOnGeom));
            reflection.startPos = reflection.startPos + 0.001 * reflection.direction;
            c = traceRay(reflection, depth) * geom->getReflect() +
                c * (1 - geom->getReflect());
        }



        return c;
    }
};

#endif
