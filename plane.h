#ifndef PLANE_H_
#define PLANE_H_

#include "linalg/linalg.h"
#include "ray.h"
#include "geometry.h"

using namespace linalg::aliases;

class Plane : public Geometry {
private:
    double3 point;
    double3 normal;


public:

    Plane(Color color, double reflect, double3 point, double3 normal):
        Geometry(color, reflect), point(point), normal(linalg::normalize(normal)) {}

    virtual double3 getNormalAt(const double3& point) const override {
        return normal;
    }

    virtual std::string getType() const override {
        return "PLANE";
    }

    virtual bool intersectsWithRay(const Ray& ray) const override {
        if (linalg::dot(ray.direction, normal) == 0) {
            // It clould be that the ray is in the plane,
            // but that is not interersting.
            return false;
        }

        if (intersectDistance(ray) < 0) {
            return false;
        }

        return true;
    }

    virtual double intersectDistance (const Ray& ray) const  override {
        return linalg::dot((point - ray.startPos), normal) /
            linalg::dot(ray.direction, normal);
    }

    virtual Color getColorAt(const double3& point) const override {
        return color;
    }

};

#endif
