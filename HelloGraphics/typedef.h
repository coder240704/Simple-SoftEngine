#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <tuple>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/AxisAngle.h>

typedef gmtl::Vec2f vec2;
typedef gmtl::Vec3f vec3;
typedef gmtl::Vec4f vec4;
typedef gmtl::Matrix33f mat3;
typedef gmtl::Matrix44f mat4;
typedef std::tuple<vec3, float> rot_axis_angle;

typedef unsigned char byte;
struct Color{
	float R;
	float G;
	float B;
	float A;
	
	Color operator*(float mul){
		return Color{ mul * this->R, mul * this->G, mul * this->B, this->A };
	}

	Color operator*(const Color &color){
		return Color{ color.R * this->R, color.G * this->G, color.B * this->B,  color.A * this->A };
	}
};

#endif