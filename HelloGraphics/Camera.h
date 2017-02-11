#ifndef CAMERA_H
#define CAMERA_H

#include "typedef.h"
#include "Transform.h"

class Camera{
public:
	vec3 Position;
	vec3 Target;
	vec3 WorldUp;

	Camera(vec3 position, vec3 target, vec3 worldUp = vec3(0.0, 1.0, 0.0));
	Camera( );
	mat4 GetViewMatrix() const;
};

#endif