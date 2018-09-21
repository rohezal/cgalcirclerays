#include "../include/circle.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace glm;

CircleHelper::CircleHelper()
{

}


std::vector<glm::dvec4> CircleHelper::getCircle(glm::dvec3 _center, glm::dvec3 _normal, double _degrees, size_t _number_of_points)
{
	std::vector<glm::dvec4> points;

	float golden_angle = 3.141592 * (3-std::sqrt(5));
    float number_of_points = _number_of_points;
	float sqr_points = std::sqrt(number_of_points-1);

    for(int i = 0; i < number_of_points; i++)
    {
		float theta = i * golden_angle;
		float r = std::sqrt(i) / sqr_points;

		points.push_back(glm::dvec4( glm::dvec3(r * std::cos(theta), 0, r * std::sin(theta)),1) );
	}

	glm::dvec3 up = glm::dvec3(0,1,0);

	float radian_divisor = 180.0f/3.141592;

	float radians = _degrees /radian_divisor;

	glm::dvec4 end = points.back();
	glm::dvec3 endpoint(end.x,end.y,end.z);

	float radius = std::sqrt(number_of_points-1) / sqr_points;

    //std::cout << "Radius: " << radius << " | Length: " << glm::length(endpoint) << std::endl;



	double distance = radius / std::tan(radians);

	glm::dvec3 normal = _normal;
	normal = glm::normalize(normal);
    //	glm::dvec3 center = _center + _normal*distance;

	glm::dvec3 n = glm::normalize(normal);

    //	float cos = glm::dot(up,n);

    //	float sin = std::sqrt(1-(cos*cos)); //1 = cos*cos+sin*sin -> 1 - cos*cos = sin*sin

	glm::dvec3 upvector = std::abs(glm::dot(n,glm::dvec3(0,1,0))) > 0.99 ? glm::dvec3(1,0,0) : glm::dvec3(0,1,0);

	glm::dvec4 u = glm::normalize(glm::dvec4(n, 0));
	glm::dvec4 v = glm::dvec4(glm::normalize(glm::cross(upvector, glm::dvec3(u.x, u.y, u.z))), 0);
	glm::dvec4 w = glm::dvec4(glm::normalize(glm::cross(glm::dvec3(u.x, u.y, u.z), glm::dvec3(v.x, v.y, v.z))), 0);
	glm::dvec4 v1(v.x,	v.y,	v.z,	0);
	glm::dvec4 v2(u.x,	u.y,	u.z,	0);
	glm::dvec4 v3(w.x,	w.y,	w.z,	0);
	glm::dvec4 v4(0,	0,		0,		1);
	glm::dmat4 transform(v1, v2, v3, v4);
	v1 = dvec4(1, 0,		0, 0);
	v2 = dvec4(0, 1,		0, 0);
	v3 = dvec4(0, 0,		1, 0);
	v4 = dvec4(0, distance, 0, 1);
	transform = transform * glm::dmat4(v1, v2, v3, v4);



	for(size_t i = 0; i < points.size(); i++){
		points[i] = transform*points[i];
		points[i] = points[i] + glm::dvec4(_center,0);
	}

    return points;


}

double CircleHelper::getDegrees(dvec3 _vector, dvec3 _normal, const dvec3 &_center)
{
	return  std::acos(glm::dot(_normal, glm::normalize(_vector-_center)))*180/3.14 ;
}

double CircleHelper::getDegrees(dvec3 _normal, const std::vector<dvec4> &_circle, const glm::dvec3& _center)
{
	float length = 0.0f;
	size_t maxindex = 0;
	glm::dvec4 center(_center,0);

    _normal = glm::normalize(_normal);

	for(size_t i = 0; i < _circle.size(); i++)
	{
		if(glm::length(_circle[i]- center) > length)
		{
			length = glm::length(_circle[i]-center);
			maxindex = i;
			//std::cout << length << " at " << maxindex << std::endl;
		}
	}

	glm::dvec3 vector (_circle[maxindex].x, _circle[maxindex].y, _circle[maxindex].z);
	vector = vector - _center;

	vector = glm::normalize(vector);
    //std::cout << vector.x << " " << vector.y << " " << " " << vector.z << std::endl;
	return CircleHelper::getDegrees(vector, _normal);
}
