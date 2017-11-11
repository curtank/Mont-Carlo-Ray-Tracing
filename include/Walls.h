#ifndef WALLS_H
#define WALLS_H

#include "Shape.h"
typedef glm::vec3 Color;
class Walls : public Shape {
public:
	Walls();
	Walls::Walls(Color leftcolor, Color rightcolor, Color upcolor, Color dowcolor, Color backcolor);
	~Walls();

	bool Intersect(const Ray &ray, float &hitDist);
	glm::vec3 GetColor(glm::vec3 &pos);
	glm::vec3 GetNormal(glm::vec3 &intersection);

protected:
	float xl, xr, yl, yr, zl, zr;
	glm::vec3 up, down, left, right, back,front;
};

#endif // WALLS_H