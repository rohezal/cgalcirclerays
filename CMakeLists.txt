project(simple_cgal)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 11)

FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(headers SOURCES ${LibFiles})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


find_package(Boost REQUIRED COMPONENTS program_options filesystem system thread)
if(Boost_PROGRAM_OPTIONS_FOUND)
    if( CGAL_AUTO_LINK_ENABLED )
        message( STATUS "Boost.ProgramOptions library: found" )
    else()
        message( STATUS "Boost.ProgramOptions library: ${Boost_PROGRAM_OPTIONS_LIBRARY}" )
    endif()

    add_definitions( "-DCGAL_USE_BOOST_PROGRAM_OPTIONS" )
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    message ("OpenMP found")
endif()

link_libraries( ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_FILE_SYSTEM_LIBRARY})

aux_source_directory("./src" SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} CGAL)



add_library(circletracer SHARED ./src/circle.cpp ./src/raymanager.cpp ./src/raytracer.cpp)
