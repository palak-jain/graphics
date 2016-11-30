#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include "Common.hpp"

/**
 * A wrapper for a GLSL shader program. Allows easy loading of vertex and fragment shaders, and setting of uniform variables.
 */
class Shader
{
  public:
    // Default constructor
    Shader();

    // Destructor
    ~Shader();

    // Load the vertex and fragment shaders from a file
    bool load(string const & vert_shader_file, string const & frag_shader_file);

    // Check if the shader has been loaded or not
    bool isLoaded() const
    {
      return loaded;
    }

    // Use the shader for rendering
    void enable();

    // Stop using the shader for rendering and revert to the fixed-function pipeline
    void disable();

    // Set uniforms in the shader (OpenGL textures can be created from Image objects using the createGLTexture function in
    // Common.hpp)
    void setUniform(string const & name, float f);
    void setUniform(string const & name, Vec4 const & v);
    void setUniform(string const & name, RGB const & color);
    void setUniform(string const & name, Mat4 const & m);
    void setUniform(string const & name, GLuint textureId);

  private:
    bool loaded, enabled;
    GLhandleARB vert_shader, frag_shader, shader_program;

}; // class Shader

#endif  // __SHADER_HPP__
