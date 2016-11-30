#ifndef __OBJPARSER_HPP__
#define __OBJPARSER_HPP__

#include "MeshInfo.hpp"
#include "Common.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

/*

THIS FILE CONTAINS THE OBJ FILE PARSER
DO NOT MODIFY!

*/

using namespace std;

/**
 * Parser that reads in files and creates MeshInfo objects.
 */
class OBJParser
{
  public:
    OBJParser();
    bool parseOBJ(string, OBJMesh *);

  private:
    int vertexCount;
    int faceCount;
    string pathTo;
    OBJGroup * currentGroup;
    int currentMaterialId;
    MTLMaterial* currentMaterial;
    int materialCount;

    bool parseOBJLine(string, OBJMesh *);
    bool parseMTL(string, OBJMesh *);
    bool parseMTLLine(string, string, OBJMesh *);
};

#endif  // __OBJPARSER_HPP__
