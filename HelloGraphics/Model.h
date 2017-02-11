#ifndef MODEL_H
#define MODEL_H

#include <json/picojson.h>
#include <string>
#include <fstream>
#include <map>
#include "Mesh.h"
#include "Material.h"

typedef picojson::array array;
typedef picojson::object object;

class Model{
public:
	static std::vector<Mesh> LoadMeshesFromJsonFile(std::string file);
};

#endif
