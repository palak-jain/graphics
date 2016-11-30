#ifndef __GLOBAL_HPP__
#define __GLOBAL_HPP__

#include "Camera.hpp"
#include "Scene.hpp"

// Global data
struct Global
{
  // The current viewing camera
  static Camera camera;

  // The scene to be displayed
  static Scene scene;

}; // struct Global

#endif  // __GLOBAL_HPP__
