#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "OBJParser.hpp"
#include "Common.hpp"
#include "Shader.hpp"

/**
 * Contains a specification of the 3D scene to be rendered.
 */
class Scene
{
  public:
    // Load the scene description from a file. Return false if the load failed.
    bool load(string const & filename, Material const & m);

    // Draw the scene using a set of OpenGL commands. Return false on error.
    bool draw();

    // Get the lighting coefficients for scene objects.
    Material const & getMaterial() const { return material; }

    Vec3 getTopOfSky();

  private:
    Material material;  // the scene material
    Shader shader;      // used to shade primitives
    OBJMesh myMesh;     // the displayed mesh

    // TODO: Put members storing mesh data etc here

}; // class Scene

#endif  // __SCENE_HPP__
