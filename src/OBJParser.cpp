#include "OBJParser.hpp"

#define PATHCHARS "\\/"

/*

THIS FILE CONTAINS THE OBJ FILE PARSER
DO NOT MODIFY (unless for extra credit)

*/

OBJParser::OBJParser()
{
}

/**
 * Attempts to parse an OBJ file. Returns NULL if we fail.
 */
bool
OBJParser::parseOBJ(string filename, OBJMesh * myMesh)
{
  vertexCount = 0;
  faceCount = 0;
  currentGroup = NULL;
  currentMaterialId = -1;

  size_t found = filename.find_last_of(PATHCHARS);
  pathTo = filename.substr(0, found);
  string file = filename.substr(found + 1);

  cout << "Loading OBJ file " << filename << endl;
  char line[1024];
  ifstream inFile(filename.c_str(), ifstream::in);
  if (!inFile)
  {
    cerr << "Could not open given obj file " << filename << endl;
    return false;
  }
  int lineCounter = 0;
  while (inFile.good())
  {
    lineCounter += 1;
    inFile.getline(line, 1023);
    if (!parseOBJLine(string(line), myMesh))
    {
      cerr << "Error on line " << lineCounter << endl;
      return false;
    }
  }
  inFile.close();

  // Yes, this is a bad place to put this.
  // Honestly, a well-designed system would separate OBJ, MTL, and the textures
  // but since we are trying to get students up and running ASAP
  // we load them the texture images into the MTLMaterial itself.
  cout << "Loading texture maps: loaded 000";
  for (unsigned int mi = 0; mi < myMesh->materials.size(); mi++)
  {
    cout << "\b\b\b";
    cout.width(3);
    cout.fill('0');
    cout << mi+1 << flush;

    Image & teximg = myMesh->materials[mi]->textureImage;
    if (!teximg.load(myMesh->materials[mi]->textureMapFilename))
    {
      cerr << "\nError loading texture image: " << myMesh->materials[mi]->textureMapFilename << endl;
      return false;
    }

    //Setup sane default colors.
    int w = teximg.width();
    int h = teximg.height();
    double avgR = 0, avgG = 0, avgB = 0;
    for (int y = 0; y < h; y++)
    {
      for (int x = 0; x < w; x++)
      {
        unsigned char const * pixel = teximg.pixel(y, x);
        avgR += pixel[0] / 255.0;
        avgG += pixel[1] / 255.0;
        avgB += pixel[2] / 255.0;
      }
    }
    avgR /= (w*h);
    avgG /= (w*h);
    avgB /= (w*h);
    RGB c(avgR, avgG, avgB);
    myMesh->materials[mi]->C = c;
  }

  myMesh->calculateBBox();

  cout << endl;
  cout << "OBJ Parser done. OBJMesh now contains: " << endl;
  cout << "  groups:         " << myMesh->groups.size() << endl;
  cout << "  faces:          " << myMesh->faces.size() << endl;
  cout << "  vertices:       " << myMesh->vertices.size() << endl;
  cout << "  texture coords: " << myMesh->vertexTexCoords.size() << endl;
  cout << "  normals:        " << myMesh->vertexNormals.size() << endl;
  cout << "  materials:      " << myMesh->materials.size() << endl;
  cout << "  bbox max:       " << myMesh->max << endl;
  cout << "  bbox min:       " << myMesh->min << endl;
  return true;
}

bool
OBJParser::parseOBJLine(string line, OBJMesh * myMesh)
{
  string operand;
  bool success = true;

  if (line.empty())
    return true;

  stringstream ss(stringstream::in | stringstream::out);
  ss.str(line);
  ss >> operand;

  if (operand[0] == '#')
  {
    return true;
  }
  else if (operand.compare("v") == 0)
  {

    double x, y, z;
    ss >>x >>y >>z;
    myMesh->vertices.push_back(OBJVertex(Vec3(x,y,z)));
    vertexCount++;

  }
  else if (operand.compare("vn") == 0)
  {

    double x, y, z;
    ss >>x >>y >>z;
    myMesh->vertexNormals.push_back(OBJVertexNormal(Vec3(x,y,z)));

  }
  else if (operand.compare("vt") == 0)
  {

    double x, y;
    ss >>x >>y;
    myMesh->vertexTexCoords.push_back(OBJVertexTexCoord(Vec2(x,y)));

  }
  else if (operand.compare("usemtl") == 0)
  {

    string material;
    ss >>material;
    bool foundMat = false;
    for (unsigned int mi = 0; mi < myMesh->materials.size(); mi++)
    {
      if (material.compare(myMesh->materials[mi]->name) == 0)
      {
        currentMaterialId = mi;
        foundMat = true;
      }
    }
    if (!foundMat)
    {
      cerr << "Could not find the material for this face!" << endl;
      return false;
    }

  }
  else if (operand.compare("mtllib") == 0)
  {

    //We do this here so that we can resolve material names as we read through.
    string mtlFileR;
    ss >>mtlFileR;
    if (!parseMTL(mtlFileR, myMesh))
    {
      return false;
    }

  }
  else if (operand.compare("g") == 0)
  {

    string name;
    currentGroup = new OBJGroup();
    ss >>name;
    currentGroup->name = name;
    myMesh->groups.push_back(currentGroup);

  }
  else if (operand.compare("f") == 0)
  {

    int v, n, t;
    OBJFace* newFace = new OBJFace();
    newFace->materialId = currentMaterialId;
    while (ss.peek() != '\n' && ss.peek() != '\r' && ss.peek() != -1)
    {
      ss >>v;
      newFace->v.push_back(v-1);
      if (ss.peek() == '/')   // we have textures and/or normals!
      {
        ss.get();
        if (ss.peek() == '/')           // we have vertex//normal
        {
          ss.get();
          ss >>n;
          newFace->vn.push_back(n-1);
        }
        else                            // we have vertex/texture[/normal]
        {
          ss >>t;
          newFace->vt.push_back(t-1);
          if (ss.peek() == '/')
          {
            ss.get();
            ss >>n;
            newFace->vn.push_back(n-1);
          }
        }
      }
    }
    myMesh->faces.push_back(newFace);
    if (currentGroup != NULL)
    {
      currentGroup->f.push_back(faceCount);
    }
    faceCount++;

  }
  else
  {
    cout << "  Unknown operand in OBJ file, skipping line: " << line << endl;
  }

  if (ss.bad())
  {
    cerr << "The bad bit of the input file's line's stringstream is set! Couldn't parse:" << endl;
    cerr << "  " << line << endl;
    success = false;
  }
  return success;
}

bool
OBJParser::parseMTL(string filename, OBJMesh * myMesh)
{
  string mtlfilename = pathTo;
  mtlfilename.append("/").append(filename);
  materialCount = 0;

  cout << "Loading MTL file " << mtlfilename << endl;
  char line[1024];
  ifstream inFile(mtlfilename.c_str(), ifstream::in);
  if (!inFile)
  {
    cerr << "Could not open given mtl file " << mtlfilename << endl;
    return false;
  }
  int lineCounter = 0;
  while (inFile.good())
  {
    lineCounter += 1;
    inFile.getline(line, 1023);
    if (!parseMTLLine(string(line), pathTo, myMesh))
    {
      cerr << "error on MTL file line " << lineCounter << endl;
      return false;
    }
  }

  inFile.close();
  cout << "MTL Parser done. Loaded " << materialCount << " materials." << endl;
  return true;
}

bool
OBJParser::parseMTLLine(string line, string pathToF, OBJMesh * myMesh)
{
  string operand;

  if (line.empty())
    return true;

  stringstream ss(stringstream::in | stringstream::out);
  ss.str(line);
  ss >> operand;

  if (operand[0] == '#')
  {
    return true; //skip comments
  }
  else if (operand.compare("") == 0 || operand.compare(" ") == 0)
  {
    return true; //skip blank lines
  }
  else if (operand.compare("newmtl") == 0)
  {

    string name;
    currentMaterial = new MTLMaterial();
    ss >>name;
    currentMaterial->name = name;
    myMesh->materials.push_back(currentMaterial);
    materialCount++;

  }
  else if (operand.compare("Kd") == 0)
  {
    if (currentMaterial == NULL)
      return false;

  }
  else if (operand.compare("illum") == 0)
  {
    if (currentMaterial == NULL)
      return false;

  }
  else if (operand.compare("map_Kd") == 0)
  {
    if (currentMaterial == NULL)
      return false;

    string textureF;
    ss >>textureF;
    currentMaterial->textureMapFilename = pathToF.append("/").append(textureF);

  }
  else
  {
    cout << "  Unknown operand in MTL file, skipping line: " << line << endl;
  }

  if (ss.bad())
  {
    cerr << "The bad bit of the input file's line's stringstream is set! Failed on line:" << endl;
    cerr << "  " << line << endl;
    return false;
  }
  return true;
}
