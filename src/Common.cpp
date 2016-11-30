#include "Common.hpp"

void
matToGL(Mat3 const & m, float * gm)
{
  gm[ 0] = m[0][0]; gm[ 4] = m[0][1]; gm[ 8] = m[0][2]; gm[12] = 0;
  gm[ 1] = m[1][0]; gm[ 5] = m[1][1]; gm[ 9] = m[1][2]; gm[13] = 0;
  gm[ 2] = m[2][0]; gm[ 6] = m[2][1]; gm[10] = m[2][2]; gm[14] = 0;
  gm[ 3] =       0; gm[ 7] =       0; gm[11] =       0; gm[15] = 1;
}

void
matToGL(Mat4 const & m, float * gm)
{
  gm[ 0] = m[0][0]; gm[ 4] = m[0][1]; gm[ 8] = m[0][2]; gm[12] = m[0][3];
  gm[ 1] = m[1][0]; gm[ 5] = m[1][1]; gm[ 9] = m[1][2]; gm[13] = m[1][3];
  gm[ 2] = m[2][0]; gm[ 6] = m[2][1]; gm[10] = m[2][2]; gm[14] = m[2][3];
  gm[ 3] = m[3][0]; gm[ 7] = m[3][1]; gm[11] = m[3][2]; gm[15] = m[3][3];
}

void
glLoadMatrix(Mat3 const & m)
{
  float gm[16];
  matToGL(m, gm);
  glLoadMatrixf(gm);
}

void
glLoadMatrix(Mat4 const & m)
{
  float gm[16];
  matToGL(m, gm);
  glLoadMatrixf(gm);
}

void
glMultMatrix(Mat3 const & m)
{
  float gm[16];
  matToGL(m, gm);
  glMultMatrixf(gm);
}

void
glMultMatrix(Mat4 const & m)
{
  float gm[16];
  matToGL(m, gm);
  glMultMatrixf(gm);
}

// Returns 1 if an OpenGL error occurred, 0 otherwise.
int
printGLError_(char const * file, int line)
{
  GLenum gl_err;
  int    ret_code = 0;

  gl_err = glGetError();
  while (gl_err != GL_NO_ERROR)
  {
    cerr << "OpenGL error in file " << file << " @ line " << line << ": " << gluErrorString(gl_err) << endl;
    ret_code = 1;
    gl_err = glGetError();
  }

  return ret_code;
}

GLuint
createGLTexture(Image const & image)
{
  int w = image.width();
  int h = image.height();
  // cout << "The size of the image is: " << w << "x" << h <<endl;

  // Now generate the OpenGL texture object
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLint format = -1;
  switch (image.numChannels())
  {
    case 1: format = GL_LUMINANCE; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default: cerr << "Image has unsupported number of channels: " << image.numChannels() << endl; return 0;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, (GLvoid const *)image.data());

  // Check if any errors occurred
  if (printGLError())
    return 0;

  return textureId;
}

void
destroyGLTexture(GLuint textureId)
{
  glDeleteTextures(1, &textureId);
}
