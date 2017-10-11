#include "scene.h"


const std::unique_ptr<Geometry>& Scene::getClosestGeom(Ray ray) const {
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

double Scene::smallestDistanceToObjectInRay(Ray ray) const {
    ray.startPos = ray.startPos + 0.001 * ray.direction;
    auto& ptr = getClosestGeom(ray);
    if (ptr.get() == nullptr) {
        return std::numeric_limits<double>::infinity();
    } else {
        return getClosestGeom(ray)->intersectDistance(ray);
    }
}

const Ray Scene::getRay(int x, int y) {

    double newX = (static_cast<double>(x)/static_cast<double>(width) - 0.5f);
    double newY = (-static_cast<double>(y)/static_cast<double>(width) +
                   static_cast<double>(height)/static_cast<double>(width)*0.5f);

    vec dir{newX, newY, 0.0f};

    return Ray(cameraPos, dir + screenPos);
}

std::vector<vec> Scene::lightsHit(const vec& point) {
    std::vector<vec> ret;
    auto pred = [&](const vec& light) {
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

Color Scene::traceRay(Ray ray, int depth) {
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

        vec internalDirection = ray.snells(1, geom->getRefraction(),
                                           geom->getNormalAt(pointOnGeom));

        Ray internal(pointOnGeom, internalDirection);

        double exitDist = geom->exitDistance(internal);
        double otherSidePower = std::pow(geom->getOpacity(), exitDist);

        vec otherSidePoint = internal.translate(exitDist+0.001);

        vec otherSideDir =
            internal.snells(geom->getRefraction(), 1,
                                     geom->getNormalAt(otherSidePoint));

        if (otherSideDir == vec(0,0,0)) {
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
