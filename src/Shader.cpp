#include "Shader.hpp"
#include <cstdio>

// Print the shader's information log if it is non-empty
void
printInfoLog(GLhandleARB obj)
{
  int infolog_length = 0;
  int chars_written  = 0;
  char * info_log;

  glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infolog_length);

  if (infolog_length > 0)
  {
    info_log = new char[infolog_length];

    glGetInfoLogARB(obj, infolog_length, &chars_written, info_log);
    cerr << info_log << endl;

    delete [] info_log;
  }
}

// Read the entire contents of a text file to a character array. The result must be freed with delete []
char *
globTextFile(string const & filename)
{
  FILE * fp = fopen(filename.c_str(), "rb");
  if (!fp) return NULL;

  fseek(fp, 0, SEEK_END);
  size_t len = (size_t)ftell(fp);

  fseek(fp, 0, SEEK_SET);
  char * buf = new char[len + 1];
  size_t bytes_read = fread(buf, 1, len, fp);
  if (bytes_read < len)
  {
    cerr << filename << ": " << len << " bytes expected, " << bytes_read << " bytes read" << endl;
    delete [] buf;
    return NULL;
  }

  // Add a terminating null character
  buf[len] = '\0';

  fclose(fp);
  return buf;
}

static bool
createShader(GLhandleARB obj, string const & filename)
{
  // Read the shader source
  char * ss = globTextFile(filename);
  if (!ss)
  {
    cerr << "Shader '" << filename << "' not found or is empty" << endl;
    return false;
  }

  // Set the shader source
  const char * ssp = ss;  // why, OpenGL spec, why?
  glShaderSourceARB(obj, 1, &ssp, NULL);
  delete [] ss;

  // Compile the shader
  glCompileShaderARB(obj);
  printInfoLog(obj);
  int success;
  glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &success);
  if (!success)
  {
    cerr << filename << ": Error compiling shader" << endl;
    return false;
  }

  return true;
}

Shader::Shader()
: loaded(false), enabled(false), vert_shader(0), frag_shader(0), shader_program(0)
{}

Shader::~Shader()
{
  if (loaded)
  {
    glDetachObjectARB(shader_program, vert_shader);
    glDetachObjectARB(shader_program, frag_shader);

    glDeleteObjectARB(shader_program);
    glDeleteObjectARB(vert_shader);
    glDeleteObjectARB(frag_shader);
  }
}

bool
Shader::load(string const & vert_shader_file, string const & frag_shader_file)
{
  if (loaded) return true;

  // Create the vertex and fragment shader objects
  vert_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
  frag_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

  // Load and compile the shader objects
  if (!createShader(vert_shader, vert_shader_file)) return false;
  if (!createShader(frag_shader, frag_shader_file)) return false;

  // Create a shader program and attach the vertex and fragment shaders
  shader_program = glCreateProgramObjectARB();
  glAttachObjectARB(shader_program, vert_shader);
  glAttachObjectARB(shader_program, frag_shader);

  // Link the program
  glLinkProgramARB(shader_program);
  printInfoLog(shader_program);
  int success;
  glGetObjectParameterivARB(shader_program, GL_OBJECT_LINK_STATUS_ARB, &success);
  if (!success)
  {
    cerr << "Error linking shaders" << endl;
    return false;
  }

  loaded = true;
  enabled = false;
  return true;
}

void
Shader::enable()
{
  if (loaded && !enabled)
  {
    glUseProgramObjectARB(shader_program);
    enabled = true;
  }
}

void
Shader::disable()
{
  glUseProgramObjectARB(0);
  enabled = false;
}

void
Shader::setUniform(string const & name, float f)
{
  if (loaded)
  {
    GLint loc = glGetUniformLocationARB(shader_program, name.c_str());
    if (loc >= 0)
      glUniform1fARB(loc, f);
    else
      cerr << "Warning: Couldn't find active uniform " << name
           << " in shader -- make sure the variable is actually used, not just declared" << endl;
  }
}

void
Shader::setUniform(string const & name, Vec4 const & v)
{
  if (loaded)
  {
    GLint loc = glGetUniformLocationARB(shader_program, name.c_str());
    if (loc >= 0)
      glUniform4fARB(loc, (float)v[0], (float)v[1], (float)v[2], (float)v[3]);
    else
      cerr << "Warning: Couldn't find active uniform " << name
           << " in shader -- make sure the variable is actually used, not just declared" << endl;
  }
}

void
Shader::setUniform(string const & name, RGB const & color)
{
  if (loaded)
  {
    GLint loc = glGetUniformLocationARB(shader_program, name.c_str());
    if (loc >= 0)
      glUniform4fARB(loc, (float)color[RED], (float)color[GREEN], (float)color[BLUE], 1);
    else
      cerr << "Warning: Couldn't find active uniform " << name
           << " in shader -- make sure the variable is actually used, not just declared" << endl;
  }
}

void
Shader::setUniform(string const & name, Mat4 const & m)
{
  if (loaded)
  {
    GLint loc = glGetUniformLocationARB(shader_program, name.c_str());
    if (loc >= 0)
    {
      float gm[16];
      matToGL(m, gm);
      glUniformMatrix4fvARB(loc, 1, GL_FALSE, gm);
    }
    else
      cerr << "Warning: Couldn't find active uniform " << name
           << " in shader -- make sure the variable is actually used, not just declared" << endl;
  }
}

void
Shader::setUniform(string const & name, GLuint textureId)
{
  if (loaded)
  {
    GLint loc = glGetUniformLocationARB(shader_program, name.c_str());
    if (loc >= 0)
    {
      cerr << "Got name " << name << endl;
      glBindTexture(GL_TEXTURE_2D, textureId);
      glUniform1iARB(loc, 0);  // this means: use the texture bound to texture unit 0
    }
    // else
    //   cerr << "Warning: Couldn't find active uniform " << name
    //        << " in shader -- make sure the variable is actually used, not just declared" << endl;
  }
}
