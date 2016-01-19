#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

// Eigen is used for linear algebra
#include <Eigen\Dense>
typedef Eigen::Vector2f vec2;
typedef Eigen::Vector3f vec3;
typedef Eigen::Vector4f vec4;
typedef Eigen::Matrix4f mat4;
typedef Eigen::Matrix3f mat3;
#include <OpenGP\GL\EigenOpenGLSupport3.h>

// Shader compiling utilities
#include <OpenGP\GL\shader_helpers.h>

// Utilities to simplify glfw setup
#include <OpenGP\GL\glfw_helpers.h>

// For mesh I/O we use OpenGP
#include <OpenGP\Surface_mesh.h>

// For GL error checking
#include "check_gl_error.h"

// These Namespaces assumed by default
using namespace std;
using namespace opengp;

// Common constants
#define PI 3.14159265359
#define WIDTH 1600
#define HEIGHT 900

