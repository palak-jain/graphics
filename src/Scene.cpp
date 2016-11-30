#include "Scene.hpp"
#include "Common.hpp"
#include <cmath>

Vec3 Scene::getTopOfSky() {
  Vec3 shift = -1*myMesh.getMeshMidpoint();
  double zshift =  sqrt(((myMesh.max.x() - myMesh.min.x())*(myMesh.max.x() - myMesh.min.x())) + ((myMesh.max.y() - myMesh.min.y())*(myMesh.max.y() - myMesh.min.y())))/2.0;
  shift[2] = myMesh.min.z() - zshift;
  return shift;
}

bool
Scene::load(string const & filename, Material const & m)
{
  material = m;
  OBJParser parser;
  bool didParse = parser.parseOBJ(filename, &myMesh);
  if (!didParse)
  {
    cerr << "Since we couldnt parse the OBJ file, we will now quit" << endl;
    exit(1);
  }
  for (unsigned int mi = 0; mi < myMesh.materials.size(); mi++)
  {
    // TODO: Replace these lines with more of your own code to process the loaded scene if necessary.
    myMesh.materials[mi]->textureId = createGLTexture(myMesh.materials[mi]->textureImage);
  
  }

  return didParse;
}

bool
Scene::draw()
{
  SaveGLState save_gl_state;  // will restore state upon destruction

  // Load the shader if it hasn't been loaded yet
  if (!shader.isLoaded())
    if (!shader.load("vert.glsl", "frag.glsl"))
      return false;

  // Use the shader
  shader.enable();

  
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  // scene material
  shader.setUniform("ma", (float)material.getMA());
  shader.setUniform("ml", (float)material.getML());
  shader.setUniform("msm", (float)material.getMSM());
  shader.setUniform("msp", (float)material.getMSP());
  shader.setUniform("ms", (float)material.getMS());

  // set light positions and components for second light
  GLfloat white8[] = {0.8f, 0.8f, 0.8f, 1.0f}, lightcolor[] = {0.3f, 0.3f, 0.3f, 1.f};

  // set light positions
  int surface = 0;
  float x = myMesh.max.x() - myMesh.min.x();
  float y = myMesh.max.y() - myMesh.min.y();
  float z = myMesh.max.z() - myMesh.min.z();
  if (x < y && x < z)surface = 0;
  else if(y < z)surface = 1;
  else surface = 2;

  GLfloat lightpos0[] = {0.0f, 0.0f, 1.0f, 0.}, lightpos1[] = { 0.f, 0.f, -1.0, 0.};;  // last cooridnate 0 : directonal , 1: point source
  // lightpos0[surface] = -1.0f;
  // lightpos1[surface] = 1.0f;
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);


  glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightcolor);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, white8);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white8);
  

  for (size_t fid = 0; fid < myMesh.faces.size(); ++fid)
  {
    OBJFace* f = myMesh.faces[fid];
    MTLMaterial* mat = myMesh.materials[f->materialId];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat->textureId);

    glBegin(GL_POLYGON);
    glColor3f(mat->C[0], mat->C[1], mat->C[2]);

    for (size_t vi = 0; vi < f->v.size(); ++vi)
    {
      
      if(f->vt[vi] != -1) {
        Vec2 const & vertexTexCoord = myMesh.vertexTexCoords[f->vt[vi]].coord;
        glTexCoord2f(vertexTexCoord.x(), vertexTexCoord.y());
      }
          shader.setUniform("frag_texture", mat->textureId);

      Vec3 const & vertex = myMesh.vertices[f->v[vi]].pos;
      glVertex3f(vertex.x(), vertex.y(), vertex.z());

    }
    glEnd();
  }


  // Don't replace this line!
  return true;
}
