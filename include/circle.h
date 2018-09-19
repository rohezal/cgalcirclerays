#ifndef CIRCLE_HELPER_H
#define CIRCLE_HELPER_H

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <fstream>      // std::ifstream
#include <iostream>

class CircleHelper
{
public:
    static std::vector<glm::dvec4> getCircle(glm::dvec3 _center, glm::dvec3 _normal, double _degrees);
    static double getDegrees(glm::dvec3 _vector,  glm::dvec3 _normal, const glm::dvec3& _center = glm::dvec3(0,0,0));
    static double getDegrees(glm::dvec3 _vector,  const std::vector<glm::dvec4> &_circle, const glm::dvec3& _center = glm::dvec3(0,0,0));

private:
    CircleHelper();
};

#endif
