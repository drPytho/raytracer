#ifndef RAY_H_
#define RAY_H_

#include "util.h"
#include "linalg/linalg.h"
#include <cmath>

#define SQRE(x) (x*x)

using namespace linalg::aliases;
//const double PI = 3.1415926;
struct Ray {
    vec startPos;
    vec direction;

    Ray(vec start, vec direction):
        startPos(start), direction(linalg::normalize(direction)) {}

    Ray(vec start, vec dirFrom, vec dirTo):
        Ray(start, dirTo - dirFrom) {}

    Ray mirrorIn(vec location, vec normal) {
        vec projection = (linalg::dot(this->direction, normal)/
                              linalg::length2(normal))*normal;
        return Ray(location, this->direction - 2.0*projection);
    }

    vec translate(double dist) const {
        return startPos + direction * dist;
    }

    vec snells (double nFrom, double nTo, vec normal) const {
        //float nue = sourceEnvironmentDensity / targetEnvironmentDensity;
        double nue = nFrom/nTo;

        // -outNormal.dotProduct(originalRayDirection);
        float cosThetaS  = -1.0 * linalg::dot(normal, direction);
        float cosThetaTSquared = 1.f - SQRE(nue) * (1.f - SQRE(cosThetaS));

        if (cosThetaTSquared < 0.f) {
            return vec(0,0,0);
        }

        vec reflectedRayDirection;
        if (cosThetaS < 0.0f) {
            reflectedRayDirection = (direction * nue) + normal *
                (nue * cosThetaS + std::sqrt(cosThetaTSquared));
        } else {
            reflectedRayDirection = (direction * nue) + normal *
                (nue * cosThetaS - std::sqrt(cosThetaTSquared));
        }
        return linalg::normalize(reflectedRayDirection);
    }
};

#endif
