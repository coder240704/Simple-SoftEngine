#ifndef VECTOR_H
#define VECTOR_H
#include "typedef.h"

#define makevec4_from_vec3(vec) vec4((vec)[0], (vec)[1], (vec)[2], 1.0f) 
#define getvec3_in_vec4(vec) vec3((vec)[0], (vec)[1], (vec)[2])
#define makevec3_from_array(arr) vec3((arr)[0], (arr)[1], (arr)[2])

#endif