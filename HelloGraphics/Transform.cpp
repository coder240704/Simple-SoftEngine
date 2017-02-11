#include "Transform.h"

mat4 Transform::GetWorldMatrix(rot_axis_angle rot, vec3 position, vec3 scale){
	auto axis = std::get<0>(rot);
	auto angle = gmtl::Math::deg2Rad(std::get<1>(rot));

	mat4 rotM = gmtl::makeRot<mat4>(gmtl::makeNormal(gmtl::AxisAnglef(angle, axis)));
	mat4 transM = GetTranslateMatrix(position);
	mat4 scaleM = gmtl::makeScale<mat4>(scale);

	return transM * scaleM * rotM;
}

mat4 Transform::GetWorldMatrix(vec3 euler, vec3 position, vec3 scale){
	auto angelX = gmtl::Math::deg2Rad(euler[0]);
	auto angelY = gmtl::Math::deg2Rad(euler[1]);
	auto angelZ = gmtl::Math::deg2Rad(euler[2]);

	mat4 rotM = gmtl::makeRot<mat4>(gmtl::EulerAngleXYZf(angelX, angelY, angelZ));
	mat4 transM = GetTranslateMatrix(position);
	mat4 scaleM = gmtl::makeScale<mat4>(scale);

	return transM * scaleM * rotM;
}

mat4 Transform::GetViewMatrix(vec3 position, vec3 target, vec3 worldUp){
	vec3 axisZ = position - target;
	gmtl::normalize(axisZ);
	vec3 axisX = gmtl::makeCross(worldUp, axisZ);
	gmtl::normalize(axisX);
	vec3 axisY = gmtl::makeCross(axisZ, axisX);
	gmtl::normalize(axisY);

	mat4 trans;
	trans[0][3] = -position[0];
	trans[1][3] = -position[1];
	trans[2][3] = -position[2];

	mat4 rot;
	rot.set(axisX[0], axisX[1], axisX[2], 0,
		       axisY[0], axisY[1], axisY[2], 0, 
		       axisZ[0], axisZ[1], axisZ[2], 0, 
		          0,            0,          0,        1);

	return rot * trans;
}

mat4 Transform::GetPerspectiveMatrix(float fovy, float aspect, float near, float far){
	assert(std::fabs(aspect - std::numeric_limits<float>::epsilon( )) > 0.0f);

	mat4 persM;
	persM.set(0.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 0.0f, 0.0f, 0.0f, 
		            0.0f, 0.0f, 0.0f, 0.0f, 
		            0.0f, 0.0f, 0.0f, 0.0f);
	float tanHalfFovy = std::tan(fovy / 2.0f);
	persM[0][0] = 1.0f / (aspect * tanHalfFovy);
	persM[1][1] = 1.0f / (tanHalfFovy);
	persM[3][2] = -1.0f;

	persM[2][2] = -(far + near) / (far - near);
	persM[2][3] = -(2.0f * far * near) / (far - near);

	return persM;
}

mat3 Transform::GetViewPortMatrix(float xl, float xh, float yl, float yh, float ul, float uh, float vl, float vh){
	mat3 mat;
	mat.set((uh - ul)/(xh - xl),           0.0f,           (ul * xh - uh * xl)/(xh - xl),
		                  0.0f,          (vh - vl)/(yh - yl),   (vl * yh - vh * yl)/(yh - yl),
						  0.0f,                     0.0f,                        0.0f);
	return mat;
}

mat4 Transform::GetTranslateMatrix(vec3 position){
	mat4 mat;
	mat[0][3] = position[0];
	mat[1][3] = position[1];
	mat[2][3] = position[2];

	return mat;
}