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

Let the raytracer start:
	
	raytracer.renderImage();

get the image from the raytracer:
	
	std::vector<double> image = raytracer.renderImage();

If you want you can save it to a file:
	
	raytracer.saveImage(); //you can provide a filename as a string





