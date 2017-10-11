#ifndef RAY_H_
#define RAY_H_

#include "linalg/linalg.h"
#include <cmath>

#define SQRE(x) (x*x)

using namespace linalg::aliases;
//const double PI = 3.1415926;
struct Ray {
    double3 startPos;
    double3 direction;

    Ray(double3 start, double3 direction):
        startPos(start), direction(linalg::normalize(direction)) {}

    Ray(double3 start, double3 dirFrom, double3 dirTo):
        Ray(start, dirTo - dirFrom) {}

    Ray mirrorIn(double3 location, double3 normal) {
        double3 projection = (linalg::dot(this->direction, normal)/
                              linalg::length2(normal))*normal;
        return Ray(location, this->direction - 2.0*projection);
    }

    double3 translate(double dist) const {
        return startPos + direction * dist;
    }

    Ray snells(double3 location, double n1, double n2, double3 normal) const {
        double n = n1/n2;
        double3 normCrossIn = linalg::cross(normal, direction);
        double3 secondPart =
          normal * std::sqrt(1 - n*n * linalg::dot(normCrossIn, normCrossIn));

        double3 firstPart = n * linalg::cross(normal, normCrossIn);

        return Ray(location, firstPart - secondPart);
    }

    double3 snellsDirection (double nFrom, double nTo, double3 normal) const {
        //float nue = sourceEnvironmentDensity / targetEnvironmentDensity;
        double nue = nFrom/nTo;

        // -outNormal.dotProduct(originalRayDirection);
        float cosThetaS  = -1.0 * linalg::dot(normal, direction);
        float cosThetaTSquared = 1.f - SQRE(nue) * (1.f - SQRE(cosThetaS));

        if (cosThetaTSquared < 0.f) {
            return double3(0,0,0);
        }

        double3 reflectedRayDirection;
        if (cosThetaS < 0.0f) {
            reflectedRayDirection = (direction * nue) + normal *
                (nue * cosThetaS + std::sqrt(cosThetaTSquared));
        } else {
            reflectedRayDirection = (direction * nue) + normal *
                (nue * cosThetaS - std::sqrt(cosThetaTSquared));
        }
        return linalg::normalize(reflectedRayDirection);
    }

    double3 snellsDirection2 (double nFrom, double nTo, double3 normal) const {
        double nue = nFrom/nTo;
        double cosTeta1 = linalg::dot(normal, -1.0 * direction);
        double cosTeta2 = std::sqrt(1 - SQRE(nue) * (1 - SQRE(cosTeta1)));
        if (cosTeta1 < 0) {
            return nue * direction + (nue*cosTeta1 + cosTeta2)*normal;
        }
        return nue * direction + (nue*cosTeta1 - cosTeta2)*normal;
    }
};

#endif
