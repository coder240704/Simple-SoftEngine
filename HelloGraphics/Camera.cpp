#include "Camera.h"

Camera::Camera( ){

}

Camera::Camera(vec3 position, vec3 target, vec3 worldUp){
	Position = position;
	Target = target;
	WorldUp = worldUp;
}

mat4 Camera::GetViewMatrix( ) const {
	return Transform::GetViewMatrix(Position, Target, WorldUp);
}