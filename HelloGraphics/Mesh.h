#ifndef MESH_H
#define MESH_H

#include "typedef.h"
#include "Transform.h"
#include "Texture.h"
#include <string>
#include <vector>

struct Face{
	int index1;
	int index2;
	int index3;
};

struct Vertex{
	vec3 Position;
	vec3 worldPosition;
	vec3 Normal;
	vec2 Texture2DCoord;
	Color color;
};

class Mesh{
public:
	std::string Name;
	Texture Texture2D;
	std::vector<Vertex> Vertices;
	std::vector<Face> Faces;
	vec3 Position;
	//rotation based on euler angle
	vec3 EulerAngleXYZ;
	//rotation based on rotation axis and rotation angle(degree)
	rot_axis_angle Rotation;

	Mesh( );
	~Mesh( );
	Mesh(std::vector<Vertex> vertices, vec3 position, vec3 euler);
	Mesh(std::vector<Vertex> vertices, std::vector<Face> faces, vec3 position, vec3 euler);
	Mesh(std::vector<Vertex> vertices, std::vector<Face> faces, vec3 position, vec3 euler, std::string filename);
	mat4 GetWorldMatrixByEuler( ) const;
};

#endif