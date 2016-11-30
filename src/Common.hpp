#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <cstdlib>
#include "core/glew.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "core/Algebra3.hpp"
#include "core/Image.hpp"
#include "core/Types.hpp"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

/*

THIS FILE CONTAINS USEFUL GLOBAL METHODS. USE THEM!

*/

// Convert a Mat3 to a flat column-major representation
void matToGL(Mat3 const & m, float * gm);

// Convert a Mat4 to a flat column-major representation
void matToGL(Mat4 const & m, float * gm);

// Convert Mat3/Mat4 to flat column-major format as required by GL, and call glLoadMatrixf/glMultMatrixf
void glLoadMatrix(Mat3 const & m);
void glLoadMatrix(Mat4 const & m);

void glMultMatrix(Mat3 const & m);
void glMultMatrix(Mat4 const & m);

// Returns 1 if an OpenGL error occurred, 0 otherwise. Call this function liberally to debug.
#define printGLError() printGLError_(__FILE__, __LINE__)

// Don't call this function directly, use printGLError() instead
int printGLError_(char const * file, int line);

// Create an OpenGL texture from an image. Remember to call the destroy function when you're done using a texture!
//
// Returns zero on error.
//
// Creating a GL texture is an expensive process, so you shouldn't do this every frame. Implement a mechanism to ensure a GL
// texture is created just once for each material during the execution of the program. You could create the GL texture when the
// OBJ is loaded from disk, or the first time it's actually used (for inspiration, see how the "isLoaded" flag of Shader
// prevents reloads).
GLuint createGLTexture(Image const & image);
void destroyGLTexture(GLuint textureId);

// An object that saves a snapshot of all current GL state when it is created, and restores the state when it is deleted.
// Follows the RAII idiom.
struct SaveGLState
{
  SaveGLState() { glPushAttrib(GL_ALL_ATTRIB_BITS); glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS); }
  ~SaveGLState() { glPopClientAttrib(); glPopAttrib(); }
};

#endif  // __COMMON_HPP__
