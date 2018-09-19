#include "../include/raymanager.h"

using namespace RashRaymanager;


Raymanager::Raymanager(Ray &_ray) : ray(_ray)
{
    this->counter=0;
    this->traveled_distance=0.0f;
}

Ray Raymanager::nextTarget(const Point& _hit, const Vector& _normal)
{
    counter++;
    traveled_distance = traveled_distance + std::sqrt(CGAL::squared_distance(_hit,ray.source()));



    Vector reflection = ray.to_vector() -
            2*(ray.to_vector() * _normal)
            * _normal;

    ray = Ray(_hit,reflection);

}

Ray &Raymanager::getRay()
{
    return ray;
}
