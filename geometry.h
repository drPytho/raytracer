#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <string>
#include "util.h"
#include "linalg/linalg.h"
#include "ray.h"
#include "color.h"

using namespace linalg::aliases;

class Geometry {
protected:
    Color color;
    double reflect;
    double opacity;
    double refraction;

public:

    Geometry(Color color, double reflect):
        color(color), reflect(reflect), opacity(0), refraction(1.54) {}

    Color getColor() const { return color; }
    void setColor(Color c) { color = c; }

    double getReflect() const { return reflect; }
    void setReflect(double r) { reflect = r; }

    double getOpacity() const { return opacity; }
    void setOpacity(double o) { opacity = o; }

    double getRefraction() const { return refraction; }
    void setRefraction(double r) { refraction = r; }

    virtual double exitDistance(const Ray&) const { return 0.0f; };
    virtual std::string getType() const = 0;
    virtual vec getNormalAt(const vec& point) const = 0;
    virtual bool intersectsWithRay(const Ray& ray) const = 0;
    virtual double intersectDistance(const Ray& ray) const = 0;
    virtual Color getColorAt(const vec& point) const = 0;
};

#endif
