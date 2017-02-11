#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "typedef.h"
#include <gmtl/Generate.h>
#include <gmtl/Math.h>
#include <gmtl/EulerAngle.h>

class Transform{
public:
	static mat4 GetWorldMatrix(rot_axis_angle rot, vec3 position, vec3 scale = vec3(1.0f, 1.0f, 1.0f));
	//euler angle sequence : x, y, z
	static mat4 GetWorldMatrix(vec3 euler, vec3 position, vec3 scale = vec3(1.0f, 1.0f, 1.0f));
	static mat4 GetViewMatrix(vec3 position, vec3 target, vec3 worldUp);
	static mat4 GetPerspectiveMatrix(float fovy, float aspect, float near, float far);
	//[xl , xh] * [ yl, yh] --> [ul, uh] * [vl, vh]
	static mat3 GetViewPortMatrix(float xl, float xh, float yl, float yh, float ul, float uh, float vl, float vh);
	static mat4 GetTranslateMatrix(vec3 position);
};

#endif