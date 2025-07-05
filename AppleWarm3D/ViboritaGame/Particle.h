#pragma once
#include "utils.h"

class Particle
{
private:
	float lifeSpan;
	float age;
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	Vec3 color;
	Vec3 toColor;
	float alpha;
	float toAlpha;
	float size;
	float toSize;
public:
	Particle(float,Vec3,Vec3,Vec3,Vec3,Vec3,float,float,float,float);
	void process(float deltaTime);
	void draw();
	bool shouldDie();
};

