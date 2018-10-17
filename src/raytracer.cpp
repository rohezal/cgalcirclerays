#include "../include/raytracer.h"


Rash::Raytracer::Raytracer(const Polyhedron &_mesh, int _degrees, int _number_of_points):
	mesh(_mesh),
	degrees(_degrees),
	focal_length(1.2f),
	origin(0,0,0),
	number_of_ray_y(1024),
    number_of_ray_x(_number_of_points),
	renderingDone(false),
	tree(faces(_mesh).first,
	faces(_mesh).second, _mesh),
    degrees_opening(_degrees)
{
	cachePrimitves();

	number_of_ray_y  = startpoints_as_vector.size();
	image.reserve(number_of_ray_y*number_of_ray_x);

    rays.reserve(number_of_ray_y*number_of_ray_x);

    /*
    std::cout << "Number of Rays " << number_of_ray_y << "*" << number_of_ray_x <<std::endl;
    for(size_t i = 0; i < number_of_ray_y*number_of_ray_x; i++)
    {
        rays.push_back( Ray(Point(0,0,0),Point(0,0,0)));
    }
    */


	initRays();
}

void Rash::Raytracer::initRays()
{
	float cos = std::cos( degreesToRadian(degrees) );

	float dx = 1.0f/focal_length/cos;
	float dy = 1.0f/focal_length/cos;

	float stepx = std::abs( 2*dx/number_of_ray_x );
	float stepy = std::abs( 2*dy/number_of_ray_y );

	Point direction(dx,dy,-1);

	/*
	#pragma omp parallel for
	for(size_t a = 0; a < number_of_ray_y; a++)
	{
		for(size_t b = 0; b < number_of_ray_x; b++)
		{
			Point start(0,0,0);
			Point end(dx-b*stepx, dy-a*stepy,-1);

			rays[transformCoordinates(a,b)] = Ray(start,end);
		}
	}
	*/



	for(size_t a = 0; a < number_of_ray_y; a++)
	{
        std::vector<glm::dvec4> circle(CircleHelper::getCircle(startpoints_as_vector[a],faceNormals_as_vector[a],degrees_opening,number_of_ray_x));

        for(size_t b = 0; b < circle.size(); b++)
		{
			Point start(startpoints_as_vector[a].x,startpoints_as_vector[a].y,startpoints_as_vector[a].z);
			//Point end(dx-b*stepx, dy-a*stepy,-1);
			Point end(circle[b].x, circle[b].y, circle[b].z);

			rays[transformCoordinates(a,b)] = Ray(start,end);
		}

        glm::dvec3 raydirection = glm::normalize( glm::dvec3(circle[circle.size()-1].x,circle[circle.size()-1].y,circle[circle.size()-1].z) - startpoints_as_vector[a]);
        glm::dvec3 lastpoint = glm::dvec3(circle[circle.size()-1].x, circle[circle.size()-1].y, circle[circle.size()-1].z);

        //std::cout << CircleHelper::getDegrees(faceNormals_as_vector[a],circle,startpoints_as_vector[a]) << std::endl;


	}

    std::cout << "Rays: " <<  rays.size() << std::endl;

}

std::vector<double> Rash::Raytracer::renderImage()
{

	if(renderingDone == true){
		return image; //rays;
	}
	else{
		#pragma omp parallel
		{
			size_t tid = omp_get_thread_num();
			size_t number_of_threads = omp_get_num_procs();
            for(size_t y = tid; y < number_of_ray_y; y=y+number_of_threads)
            {
                if(y >= number_of_ray_y)
                {
					break;
				}

                for(size_t x = 0; x < number_of_ray_x; x++)
                {
					Ray_intersection hit = tree.first_intersection(rays[this->transformCoordinates(y,x)]);

                    if(hit)
                    {
						const Point& point =  boost::get<Point>(hit->first);
						const Primitive_id& primitive_id = boost::get<Primitive_id>(hit->second);

						Polyhedron::Halfedge_around_facet_circulator facerunner = primitive_id->facet_begin();

						Point p1;
						Point p2;
						Point p3;

						p1 = facerunner->vertex()->point();
						facerunner++;
						p2 = facerunner->vertex()->point();
						facerunner++;
						p3 = facerunner->vertex()->point();

						Vector v1(p1,p2);
						Vector v2(p1,p3);

						Vector n = CGAL::cross_product(v1,v2);
                        n = n/std::sqrt(n.squared_length());

						const CGAL::Vector_3<Kernel> eye(Ray(Point(0,0,0), Point(0,0,-1)));
						CGAL::Vector_3<Kernel> ray(rays[this->transformCoordinates(y,x)]);

                        double distance = CGAL::squared_distance(point, rays[this->transformCoordinates(y,x)].source());

                        //image[transformCoordinates(y,x)]=1.0f * std::abs((n/n.squared_length() * ray));
                        //image[transformCoordinates(y,x)]=distance;
                        image[transformCoordinates(y,x)]=distance;
					}

                    else
                    {
                        //std::cout << "This should not happen" << std::endl;
                        miss_counter++;
						image[transformCoordinates(y,x)]=0.0f;
					}
				}
			}
		}

		renderingDone = true;
        std::cout << "renderingDone " << renderingDone  << " | Rays Size: " << rays.size() << std::endl;
		return image;
    }
}

std::vector<std::vector<double> > Rash::Raytracer::render2DImage()
{
    renderImage();
    std::vector<std::vector<double> > image2d;
    //image2d.push_back(std::vector<double>());


    std::vector<double>::iterator it = image.begin();

    const size_t stepsize = this->number_of_ray_x;
    for(size_t i = 0; i < this->number_of_ray_y; i++)
    {

        image2d.push_back(std::vector<double>());
        std::copy(it, it+stepsize,std::back_inserter(image2d[i]));
        it = it+stepsize;
        //std::copy()
    }

    return image2d;


}

void Rash::Raytracer::initCircle(float radius, int numberOfPoints){

}

std::vector<Rash::Point> Rash::Raytracer::translateCircle(const Rash::Point &center, const Rash::Vector &_normal){

}

std::vector<double> Rash::Raytracer::getMin(std::vector<std::vector<double> > &_image2d)
{
    std::vector<double> mins;

    for(size_t i = 0; i < _image2d.size(); i++)
    {
        std::vector<double> temp = _image2d[i];
        std::sort(temp.begin(), temp.end());
        mins.push_back(temp.front());
    }

    return mins;
}

std::vector<double> Rash::Raytracer::getMax(std::vector<std::vector<double> > &_image2d)
{
    std::vector<double> mins;

    for(size_t i = 0; i < _image2d.size(); i++)
    {
        std::vector<double> temp = _image2d[i];
        std::sort(temp.begin(), temp.end());
        mins.push_back(temp.back());
        //std::cout << "Max: " << temp.back() << std::endl;
    }

    return mins;
}

std::vector<double> Rash::Raytracer::getMedian(std::vector<std::vector<double> > &_image2d)
{
    std::vector<double> mins;

    for(size_t i = 0; i < _image2d.size(); i++)
    {
        std::vector<double> temp = _image2d[i];
        std::sort(temp.begin(), temp.end());
        mins.push_back(temp[temp.size()/2]);
    }

    return mins;
}

std::vector<double> Rash::Raytracer::getAverage(std::vector<std::vector<double> > &_image2d)
{
    std::vector<double> mins;

    for(size_t i = 0; i < _image2d.size(); i++)
    {
        std::vector<double>& temp = _image2d[i];
        double avg = 0;
        for (size_t x = 0; x < temp.size(); x++)
        {
            avg += temp[x];
        }
        avg /= temp.size();

        mins.push_back(avg);
    }

    return mins;
}

void Rash::Raytracer::renderHitpoints(){
	std::cout << "Number of facets " << mesh.size_of_facets() << std::endl;

	//for(auto i = mesh.facets_begin(); i != mesh.facets_end(); i++)


}

void Rash::Raytracer::saveHitpoints(const std::string &_filename){

}

void Rash::Raytracer::initHitRays(){

}

void Rash::Raytracer::saveImage(const std::__cxx11::string &_filename){
	std::ofstream file(_filename.c_str());

	file << "P2" << std::endl << number_of_ray_x << " " << number_of_ray_y << std::endl << "255" << std::endl;

	for(size_t y = 0; y < number_of_ray_y; y++)
	{
		for(size_t x = 0; x < number_of_ray_x; x++)
		{
			file << (int) (image[transformCoordinates(y,x)]*255) << ' ';
		}
		file << std::endl;
	}
}

void Rash::Raytracer::cachePrimitves(){
	for(auto i = faces(mesh).begin(); i != faces(mesh).end(); i++)
	{
		primitives.push_back((*i));
	}


	for(size_t i = 0; i < primitives.size(); i++)
	{
		const Primitive_id& primitive_id = primitives[i];

		Polyhedron::Halfedge_around_facet_circulator facerunner = primitive_id->facet_begin();

		Point p1;
		Point p2;
		Point p3;

		p1 = facerunner->vertex()->point();
		facerunner++;
		p2 = facerunner->vertex()->point();
		facerunner++;
		p3 = facerunner->vertex()->point();

		Vector v1(p1,p2);
		Vector v2(p1,p3);

        Vector n = (-1)*CGAL::cross_product(v1,v2);
        n = n / std::sqrt(n.squared_length());
        Point centroid = CGAL::centroid(p1,p2,p3) + n*0.00001;

        faceNormals[primitive_id] = n;
		startpoints[primitive_id] = centroid;
        startpoints_as_vector.push_back(glm::dvec3(centroid.x(),centroid.y(), centroid.z()));
        faceNormals_as_vector.push_back(glm::normalize(glm::dvec3(n.x(),n.y(),n.z())));
	}
}
