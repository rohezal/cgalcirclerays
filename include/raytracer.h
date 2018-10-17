#ifndef CGAL_RAYTRACER_RASH
#define CGAL_RAYTRACER_RASH

#include <omp.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/print_wavefront.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>

#include <CGAL/centroid.h>
#include "circle.h"




namespace Rash
{
    typedef CGAL::Simple_cartesian<double>     Kernel;
    typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
    typedef Polyhedron::HalfedgeDS             HalfedgeDS;
    typedef Polyhedron::Vertex                 Vertex;
    typedef Kernel::Ray_3                      Ray;
    typedef Kernel::Point_3                    Point;

    typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron> Primitive;
    typedef CGAL::AABB_traits<Kernel, Primitive> Traits;
    typedef CGAL::AABB_tree<Traits> Tree;
    typedef Tree::Primitive_id Primitive_id;

    typedef CGAL::Surface_mesh<Point> Mesh;
    typedef boost::optional<Tree::Intersection_and_primitive_id<Ray>::Type> Ray_intersection;
    typedef boost::graph_traits<Mesh>::face_descriptor face_descriptor;

    typedef Traits 	Geom_traits;
    typedef CGAL::Vector_3<Kernel> Vector;

//AABB_face_graph_triangle_primitive< FaceGraph, VertexPointPMap, OneFaceGraphPerTree, CacheDatum >




    struct Skip
    {
      face_descriptor fd;
      Skip(const face_descriptor fd)
        : fd(fd)
      {}
      bool operator()(const face_descriptor& t) const
      { if(t == fd){
          std::cerr << "ignore " << t  <<std::endl;
        };
        return(t == fd);
      }

    };

    class Raytracer
    {
        public:

        const int degrees;
        const float focal_length;
        std::vector<Ray> rays;
        std::vector<double> image;
        std::vector<Point> hitpoints;
        std::map<Primitive_id, Point> startpoints;
        std::vector<glm::dvec3> startpoints_as_vector;
        std::vector<Ray> hitRays;
        std::vector<Primitive_id> primitives;
        std::vector<glm::vec3> circle;

        Point origin;
        Tree tree;
        int number_of_ray_x;
        int number_of_ray_y;
        bool renderingDone;
        const Polyhedron& mesh;
        float degrees_opening;

        size_t miss_counter = 0;

        std::map<Primitive_id, Vector> faceNormals; //face, normal
        std::vector<glm::dvec3> faceNormals_as_vector; //face, normal

        Raytracer(const Polyhedron& _mesh, int _degrees, int _number_of_points = 256);
        void initRays();
        std::vector<double> renderImage();
        std::vector<std::vector<double> > render2DImage();

        void initCircle(float radius, int numberOfPoints=64);
        std::vector<Point> translateCircle(const Point& center , const Vector& _normal);

        inline int transformCoordinates(int y, int x) const
        {
            return number_of_ray_x*y+x;
        }

        inline float degreesToRadian(int _degrees)
        {
            return _degrees * 3.14159f / 180;
        }

        size_t getWidth() const
        {
            return number_of_ray_x;
        }

        size_t getHeight() const
        {
            return number_of_ray_y;
        }

        static std::vector<double> getMin(std::vector<std::vector<double> >& _image2d);
        static std::vector<double> getMax(std::vector<std::vector<double> >& _image2d);
        static std::vector<double> getMedian(std::vector<std::vector<double> >& _image2d);
        static std::vector<double> getAverage(std::vector<std::vector<double> >& _image2d);

        void renderHitpoints();

        void saveHitpoints(const std::string& _filename = "hitpoints.xyz");
        void initHitRays();

        void saveImage(const std::string& _filename = "image.ppm");

        void cachePrimitves();
    };
}


#endif
