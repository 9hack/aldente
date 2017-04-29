#pragma once

#include <GL/glew.h>
#include "geometry.h"

#include <vector>
#include <time.h>

#include "util/util.h"

/*
	A utility class containing functions for generating primitive 3D models with ease.
*/
class GeometryGenerator {
public:
    static std::vector<Geometry *> geometries;

    static void destroy();

    static Geometry *generate_cube(GLfloat scale);

    static Geometry *generate_sphere(GLfloat radius, GLuint divisions);

    static Geometry *generate_cylinder(GLfloat radius, GLfloat height, GLuint divisions, bool is_centered);

    static Geometry *generate_plane(GLfloat scale, bool has_texture);
};