#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Common.hpp"

/**
 * A camera for viewing the scene.
 */
class Camera
{
  public:
    // Default constructor
    //
    // fov_ - Field of view in degrees
    // near_ - Distance to near plane (positive)
    // far_ - Distance to far plane (positive)
    //
    Camera(float fov_, float near_, float far_)
      : fov(fov_), near(near_), far(far_), camera_to_world(identity3D()), world_to_camera(identity3D())
    {}

    // Get projection parameters
    float getFOV() const
    {
      return fov;
    }

    float getNearDistance() const
    {
      return near;
    }

    float getFarDistance() const
    {
      return far;
    }

    // Set projection parameters
    void setFOV(float fov_)
    {
      fov = fov_;
    }

    void setNearDistance(float near_)
    {
      near = near_;
    }

    void setFarDistance(float far_)
    {
      far = far_;
    }

    // Set the camera-to-world transform (automatically computes the inverse transform as well)
    void setCameraToWorld(Mat4 const & c2w)
    {
      camera_to_world = c2w;
      world_to_camera = c2w.inverse();
    }

    // Set the world-to-camera transform (automatically computes the inverse transform as well)
    void setWorldToCamera(Mat4 const & w2c)
    {
      world_to_camera = w2c;
      camera_to_world = w2c.inverse();
    }

    // Get the camera-to-world transfrom
    Mat4 const & getCameraToWorld() const
    {
      return camera_to_world;
    }

    // Get the world-to-camera transfrom
    Mat4 const & getWorldToCamera() const
    {
      return world_to_camera;
    }

    // Get the direction we're looking in (works as long as the camera transform is a rigid transform or a uniform scaling,
    // i.e. no shearing etc). This is the camera's negative-Z axis, in world coordinates.
    Vec3 getLookVector() const
    {
      Vec3 dir(-camera_to_world[0][2], -camera_to_world[1][2], -camera_to_world[2][2]);
      dir.normalize();
      return dir;
    }

  private:
    // Field of view of the camera, in degrees
    float fov;

    // Near and far clipping plane distances (positive values, i.e. absolute values of camera-space Z)
    float near, far;

    // Transforms between camera space and world space
    Mat4 camera_to_world;
    Mat4 world_to_camera;

}; // class Camera

#endif  // __CAMERA_HPP__
