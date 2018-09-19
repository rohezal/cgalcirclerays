#ifndef CGAL_OBJLOADER_RASH
#define CGAL_OBJLOADER_RASH

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/print_wavefront.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>


namespace INTERNAL_CGAL_OBJLOADER_RASH {

typedef CGAL::Simple_cartesian<double>     Kernel;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Polyhedron::HalfedgeDS             HalfedgeDS;
typedef Polyhedron::Vertex                 Vertex;

}


template <class HDS>
class ObjLoader : public CGAL::Modifier_base<HDS>
{
public:
    ObjLoader(const std::string& _filename)
    {
        filename = _filename;
    }
    std::string filename;
    int num_faces;
    int num_vertices;
    CGAL::Polyhedron_incremental_builder_3<HDS>* p_builder;
    /*
    void operator()(const std::string& _filename, HDS& _mesh)
    {
            filename = _filename;
            this->operator ()(_mesh);
    }
    */

    void operator()(HDS& _mesh)
    {
        num_faces = 0;
        num_vertices = 0;

        readMetadata();

        std::cout << "Vertices " << num_vertices << " | Faces: " << num_faces << std::endl;

        CGAL::Polyhedron_incremental_builder_3<HDS> builder( _mesh,true);
        builder.begin_surface(num_vertices,num_faces);


        p_builder = &builder;

        std::ifstream file(filename.c_str());
        std::string line;

        while (std::getline(file, line))
        {
            if(line.at(0) == '#')
            {
                std::cout << "Ignoring " << line << std::endl;
            }
            else if(line.at(0) == 'v')
            {
                processVertices(line);
            }
            else if(line.at(0) == 'f')
            {
                processFaces(line);
            }
        }

        builder.end_surface();
    }

    void processFaces(const std::string& line)
    {
        if(line.at(0) != 'f')
        {
            std::cout << "Face did not start with f " << line << std::endl;
            return;
        }

        std::vector<std::string> face_components = split(line, " ");

        p_builder->begin_facet();
        p_builder->add_vertex_to_facet( std::atoi(face_components[1].c_str())-1);
        p_builder->add_vertex_to_facet( std::atoi(face_components[2].c_str())-1);
        p_builder->add_vertex_to_facet( std::atoi(face_components[3].c_str())-1);
        p_builder->end_facet();
    }

    void processVertices(const std::string& line)
    {
        if(line.at(0) != 'v')
        {
            std::cout << "Vertex did not start with v " << line << std::endl;
            return;
        }

        std::vector<std::string> vertex_components = split(line, " ");

        INTERNAL_CGAL_OBJLOADER_RASH::Kernel::Point_3 point( std::atof(vertex_components[1].c_str()), std::atof(vertex_components[2].c_str()), std::atof(vertex_components[3].c_str())-0.2f );
        p_builder->add_vertex(point);
    }

    void readMetadata()
    {
        std::ifstream file(filename.c_str());
        std::string line;

        while (std::getline(file, line))
        {
            if(line.at(0) == 'v')
            {
                num_vertices++;
            }
            if(line.at(0) == 'f')
            {
                num_faces++;
            }

        }
        file.close();
    }

    static void saveObj(const CGAL::Polyhedron_3<INTERNAL_CGAL_OBJLOADER_RASH::Kernel>& mesh, const std::string& _filename)
    {

        std::ofstream ofs(_filename.c_str());
        CGAL::print_polyhedron_wavefront(ofs,mesh);
        ofs.close();
    }

    static std::vector<std::string> split(const std::string& _string, const std::string& _seperator)
    {
        std::vector<std::string> strings;

        const std::string& s = _string;

        size_t start = 0U;

        for(size_t i = 0; i < s.length(); i++)
        {
            if(s.at(i) == _seperator.at(0)  || i+1 == s.length())
            {
                //std::std::cout << s.substr(start, i-start) << endl;
                strings.push_back(s.substr(start, i-start));
                start = i+1;
            }
        }

        return strings;
    }
};

#endif
