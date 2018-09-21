# cgalcirclerays
Ray Tracer which uses circles from centroids to shoot rays up to certain angles into the mesh. 

Needs libglm-dev , libcgal (in some distros maybe with extensions) and libboost-all-dev.

How to use it:

Load a modell and if needed build a convex hull:

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
	


Than give the cgal mesh to the raytracer:

    Rash::Raytracer raytracer(convexHull,30,1024); //mesh, opening angle, number of points in the circle - sampling points

Get the image from the raytracer. A raytracer caches the image and won't calculate it again. If you want to calculate an other image, use a new raytracer:
	
	std::vector<double> image = raytracer.renderImage(); //45 seconds
        std::vector<double> image2 = raytracer.renderImage(); //1 second, uses the cached image

If you want you can save the cached image to a file:
	
	raytracer.saveImage(); //you can provide a filename as a string





