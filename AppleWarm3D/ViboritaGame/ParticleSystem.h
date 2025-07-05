#pragma once
#include "utils.h"
#include "Particle.h"

class ParticleSystem
{
private:
	//Generation params ranges:
	float lifeSpanRange[2];
	Vec3 posRange[2];
	Vec3 vel[2];
	Vec3 acc[2];
	Vec3 color[2];
	Vec3 cosWave[2]; //To add some jiggle jiggle
	float alpha[2];
	float size[2];

	//Particle system thingies
	std::list<Particle*> particles;
	float genSpeed;
	float timeSinceLastSpawn;
	int maxParticles;
	float lifeSpan;
	float age;
public:
	ParticleSystem(float genSpeed,float lifeSpan, int maxParticles);
	void process(float deltaTime);
	void draw();

	void generateParticle();
	bool shouldDissapear();

	// Position Range
	Vec3 getMinPosRange() const { return posRange[0]; }
	Vec3 getMaxPosRange() const { return posRange[1]; }
	void setPosRange(Vec3 min, Vec3 max) { posRange[0] = min; posRange[1] = max; }

	// Life Span
	float getMinLifeSpan() const { return lifeSpanRange[0]; }
	float getMaxLifeSpan() const { return lifeSpanRange[1]; }
	void setLifeSpan(float min, float max) { lifeSpanRange[0] = min; lifeSpanRange[1] = max; }

	// Velocity
	Vec3 getMinVelocity() const { return vel[0]; }
	Vec3 getMaxVelocity() const { return vel[1]; }
	void setVelocityRange(const Vec3 min, const Vec3 max) { vel[0] = min; vel[1] = max; }

	// Acceleration
	Vec3 getMinAcceleration() const { return acc[0]; }
	Vec3 getMaxAcceleration() const { return acc[1]; }
	void setAccelerationRange(const Vec3 min, const Vec3 max) { acc[0] = min; acc[1] = max; }

	// Color
	Vec3 getStartColor() const { return color[0]; }
	Vec3 getEndColor() const { return color[1]; }
	void setColorRange(const Vec3 start, const Vec3 end) { color[0] = start; color[1] = end; }

	// Cosine Wave (jiggle)
	Vec3 getMinCosWave() const { return cosWave[0]; }
	Vec3 getMaxCosWave() const { return cosWave[1]; }
	void setCosWaveRange(const Vec3 min, const Vec3 max) { cosWave[0] = min; cosWave[1] = max; }

	// Alpha
	float getStartAlpha() const { return alpha[0]; }
	float getEndAlpha() const { return alpha[1]; }
	void setAlphaRange(float start, float end) { alpha[0] = start; alpha[1] = end; }

	// Size
	float getStartSize() const { return size[0]; }
	float getEndSize() const { return size[1]; }
	void setSizeRange(float start, float end) { size[0] = start; size[1] = end; }
};

