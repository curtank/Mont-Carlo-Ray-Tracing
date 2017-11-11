#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "Image.h"

class Camera {
public:
	Camera();
	Camera(float fov,glm::vec3 orignposition, glm::vec3 nvector);
	~Camera();
	Ray GetRayDirection(int x, int y, int p, Image* img);
	Ray GetRandomRayDirection(int x, int y, Image* img);

protected:
	float fov;
	float theta;
	float angle;
	glm::vec3 orign;
	glm::vec3 nvector;
	glm::mat4 transformMat;
};

#endif // CAMERA_H