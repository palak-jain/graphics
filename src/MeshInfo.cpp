#include "MeshInfo.hpp"
#include <sstream>
#include <fstream>
#include <limits>

void
OBJMesh::clear()
{
  for (vector<OBJGroup*>::iterator it = groups.begin(); it != groups.end(); ++it)
    delete *it;
  for (vector<OBJFace*>::iterator it = faces.begin(); it != faces.end(); ++it)
    delete *it;
  for (vector<MTLMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it)
    delete *it;
}

Vec3
OBJMesh::getMeshMidpoint()
{
  Vec3 midP((max[0] + min[0]) / 2.0, (max[1] + min[1]) / 2.0, (max[2] + min[2]) / 2.0);
  return midP;
}

void
OBJMesh::calculateBBox()
{
  min[0] = min[1] = min[2] = numeric_limits<double>::max();
  max[0] = max[1] = max[2] = -numeric_limits<double>::max();

  for (size_t vi = 0; vi < vertices.size(); vi++)
  {
    Vec3 v = vertices[vi].pos;
    if (v[0] < min[0])
      min[0] = v[0];
    if (v[1] < min[1])
      min[1] = v[1];
    if (v[2] < min[2])
      min[2] = v[2];

    if (v[0] > max[0])
      max[0] = v[0];
    if (v[1] > max[1])
      max[1] = v[1];
    if (v[2] > max[2])
      max[2] = v[2];
  }
}
