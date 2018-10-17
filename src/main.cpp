#include "../include/cgal_objloader.h"
#include "../include/raytracer.h"
#include <ctime>



typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;
typedef Polyhedron::Vertex                 Vertex;

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Polyhedron_3<Kernel>         ConvexHull;
typedef CGAL::Point_3<Kernel>                      Point;




int main()
{
	Polyhedron mesh;
	ObjLoader<HalfedgeDS> objLoader("bunny.obj");
	mesh.delegate(objLoader);

	ObjLoader<HalfedgeDS>::saveObj(mesh,"cgalmesh.obj");

	ConvexHull convexHull;

	std::list<Point> points;

	//CGAL::convex_hull_3(mesh.vertices_begin() , mesh.vertices_end(), convexHull);

	for(Polyhedron::Vertex_const_iterator i = mesh.vertices_begin(); i != mesh.vertices_end(); i++){
		points.push_back(i->point());
	}

	CGAL::convex_hull_3(points.begin() , points.end(), convexHull);

    Rash::Raytracer raytracer(convexHull,30,1024); //mesh, opening angle, number of points in the circle - sampling points

    //raytracer.renderHitpoints();

	double start = omp_get_wtime();
	raytracer.renderImage();
	double end = omp_get_wtime();

	double elapsed_secs = end - start;
	std::cout << "Time " << elapsed_secs << std::endl;

    std::vector<double> image = raytracer.renderImage();
	raytracer.saveImage();

    std::cout << "Misses :" << raytracer.miss_counter << " | Rays: " << raytracer.rays.size() << std::endl;


    std::vector<std::vector<double> > image2d = raytracer.render2DImage();

    std::cout << "2D Image y: " <<  image2d.size() << " x: " << image2d.front().size() << std::endl;

    std::vector<double> maxs = Rash::Raytracer::getMax(image2d);
    std::vector<double> mins =Rash::Raytracer::getMin(image2d);
    std::vector<double> meds =Rash::Raytracer::getMedian(image2d);
    std::vector<double> avgs =Rash::Raytracer::getAverage(image2d);

    for (auto a : maxs) {
        std::cout << a << " ";
    }
    std::cout << '\n';

    system("eog image.ppm") ;



	return 0;
}
