#ifndef CGAL_RAYMANAGER_RASH
#define CGAL_RAYMANAGER_RASH

#include <omp.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

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





namespace RashRaymanager
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

    class Raymanager
    {
        public:
        Ray ray;
        int counter;
        float traveled_distance;

        Raymanager(Ray& _ray);
        Ray nextTarget(const Point& _hit, const Vector& _normal);
        Ray& getRay();
    };
}


#endif
