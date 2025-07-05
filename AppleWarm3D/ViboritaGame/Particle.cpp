#include "Particle.h"

Particle::Particle(float lifeSpan, Vec3 pos, Vec3 vel, Vec3 acc, Vec3 color, Vec3 toColor, float alpha, float toAlpha, float size, float toSize)
{
	this->lifeSpan = lifeSpan;
	this->age = 0;
	this->pos = pos;
	this->vel = vel;
	this->acc = acc;
	this->color = color;
	this->toColor = toColor;
	this->alpha = alpha;
	this->toAlpha = toAlpha;
	this->size = size;
	this->toSize = size;
}

void Particle::process(float deltaTime)
{
	age += deltaTime;
	float progress = (lifeSpan - age) / lifeSpan;

	//dark deltaTime magic
	vel.x += acc.x * deltaTime * 0.5f;
	vel.y += acc.y * deltaTime * 0.5f;
	vel.z += acc.z * deltaTime * 0.5f;

	pos.x += vel.x *deltaTime;
	pos.y += vel.y *deltaTime;
	pos.z += vel.z *deltaTime;

	vel.x += acc.x * deltaTime * 0.5f;
	vel.y += acc.y * deltaTime * 0.5f;
	vel.z += acc.z * deltaTime * 0.5f;

	float alphaDiff = toAlpha - alpha;
	alpha += progress * alphaDiff * deltaTime;
	float sizeDiff = toSize - size;
	size += progress * sizeDiff * deltaTime;
	Vec3 colorDiff = subtractV3(toColor,color);
	color.x += progress * colorDiff.x * deltaTime;
	color.y += progress * colorDiff.y * deltaTime;
	color.z += progress * colorDiff.z * deltaTime;
}

void Particle::draw()
{
	glPointSize(size); 
	glBegin(GL_POINTS);
	glColor4f(color.x, color.y, color.y,alpha);
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();
	glColor3f(1, 1, 1);
}

bool Particle::shouldDie()
{
	return age > lifeSpan;
}
