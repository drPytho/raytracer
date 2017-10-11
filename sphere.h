#ifndef SPHERE_H_
#define SPHERE_H_

#include <cmath>
#include "linalg/linalg.h"
#include "ray.h"
#include "geometry.h"

using namespace linalg::aliases;

class Sphere : public Geometry {
private:
    double3 center;
    double diameter;

public:

    Sphere(Color color, double reflect, double3 center, double diameter):
        Geometry(color, reflect), center(center), diameter(diameter) {}

    virtual double3 getNormalAt(const double3& point) const override {
        return linalg::normalize(point - center);
    }

    double exitDistance(const Ray& ray) const {
        double a = -(linalg::dot(ray.direction, ray.startPos - center));
        double b = a*a - linalg::distance2(center, ray.startPos) + diameter*diameter;
        return a + sqrt(b);
    }

    virtual std::string getType() const override {
        return "SPHERE";
    }

    virtual bool intersectsWithRay(const Ray& ray) const override {
        double a = -(linalg::dot(ray.direction, ray.startPos - center));
        if (a*a + diameter*diameter < linalg::distance2(center, ray.startPos)) {
            return false;
        }
        if (intersectDistance(ray) <= 0) {
            return false;
        }
        return true;
    }

    virtual double intersectDistance (const Ray& ray) const override {
        double a = -(linalg::dot(ray.direction, ray.startPos - center));
        double b = a*a - linalg::distance2(center, ray.startPos) + diameter*diameter;
        return a - sqrt(b);
    }

    virtual Color getColorAt(const double3& point) const override {
        return color;
    }
};

#endif
