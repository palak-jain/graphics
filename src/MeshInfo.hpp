/*
 * MeshInfo.hpp
 *
 * Basic mesh data
 *
 *  Created on: Feb 19, 2009
 *      Author: jima
 */

#ifndef __MESH_INFO_HPP__
#define __MESH_INFO_HPP__

#include "Common.hpp"
#include <vector>

/*

THIS FILE CONTAINS THE BASIC MESH THAT YOU WILL BE RENDERING.
THIS IS POPULATED BY OBJPARSER
DO NOT TAKE ANYTHING OUT OF THIS FILE!
IF YOU FEEL THE NEED TO ADD DATA TO IT, YOU CAN.

*/

struct OBJVertex
{
  Vec3 pos;
  OBJVertex(Vec3 const & pos_) : pos(pos_) {}
};

struct OBJVertexTexCoord
{
  Vec2 coord;
  OBJVertexTexCoord(Vec2 const & coord_) : coord(coord_) {}
};

struct OBJVertexNormal
{
  Vec3 dir;
  OBJVertexNormal(Vec3 const & dir_) : dir(dir_) {}
};

struct OBJFace
{
  vector<int> v;   //Contains the index of the vertex in the vertices list
  vector<int> vt;  //Contains the index of the texture coord, or -1 if this triangle doesn't have texture coords.
  vector<int> vn;  //Contains the index of the normals, or -1 if this triangle doesn't have precomputed normals.
   int materialId;  //Contains an index into OBJMesh.materials for the texture map of this face (or -1 if it has none)
};

struct OBJGroup
{
  string name;
  vector<int> f;
};

struct MTLMaterial
{
  string name;                //Contains the name of the material, so you can identify it
  string textureMapFilename;  //Contains the texturemap's filename
  Image textureImage;         //Contains the actual texturemap bitmap data
  RGB C;                      //Contains the color of this material
  GLuint textureId;           //You can use this to store the OpenGL texture id of this texture.

  MTLMaterial() : textureId(0) {}
};

struct OBJMesh
{
  vector<OBJGroup*> groups;      //You probably dont need this. Groups of vertices into larger objects.
  vector<OBJFace*> faces;        //This is important! All the faces in the mesh (aka, triangles, quads, so forth)
  vector<OBJVertex> vertices;   //All the vertices. Access this with the indices in faces.
  vector<OBJVertexTexCoord> vertexTexCoords; //All the texture coordinates. Access this with the indices in faces.
  vector<OBJVertexNormal> vertexNormals;     //All the normals. This will be empty for this assignment.
  vector<MTLMaterial*> materials;  //Stores information about the material, specifically the texture maps.
                                   //Index by face's materialId

  Vec3 min; //Bounding box. Might come in handy to select a default position.
  Vec3 max;

  ~OBJMesh()
  {
    clear();
  }

  void clear();
  Vec3 getMeshMidpoint();
  void calculateBBox();  //This is called from OBJParser just before it exits to populate min and max.
};

#endif  // __MESH_INFO_HPP__
