#include "Mesh.h"

Mesh::Mesh( ){
	
}

Mesh::~Mesh(){
	
}

Mesh::Mesh(std::vector<Vertex> vertices, vec3 position, vec3 euler){
	Vertices = vertices;
	Position = position;
	EulerAngleXYZ = euler;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Face> faces, vec3 position, vec3 euler){
	Vertices = vertices;
	Position = position;
	EulerAngleXYZ = euler;
	Faces = faces;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Face> faces, vec3 position, vec3 euler, std::string filename){
	Vertices = vertices;
	Position = position;
	EulerAngleXYZ = euler;
	Faces = faces;
	Texture2D = Texture(filename);
}

mat4 Mesh::GetWorldMatrixByEuler( ) const {
	return Transform::GetWorldMatrix(EulerAngleXYZ, Position);
}

