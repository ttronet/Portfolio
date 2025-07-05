#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float genSpeed, float lifeSpan, int maxParticles)
{
	this->genSpeed = genSpeed;
	this->lifeSpan = lifeSpan;
	this->maxParticles = maxParticles;
}

void ParticleSystem::process(float deltaTime)
{
	age += deltaTime;
	timeSinceLastSpawn += deltaTime;
	if (age < lifeSpan &&
		maxParticles > particles.size() && 
		timeSinceLastSpawn > genSpeed)
	{
		generateParticle();
		timeSinceLastSpawn = 0;
	}

	std::list<Particle*>::iterator it;
	it = particles.begin();

	while (!particles.empty() && it != particles.end()) {
		Particle* p = (*it);
		p->process(deltaTime);
		p->draw();
	
		if (p->shouldDie()) {
			delete p;
			it = particles.erase(it); //O(1) baby!!
		}else
			it++;
	}
}

void ParticleSystem::draw()
{
	for (Particle* p : particles)
		p->draw();
}

bool vec3HasValue(Vec3 v) {
	return v.x != 0 || v.y != 0 || v.z != 0;
}
Vec3 getRandomVec3(Vec3 from, Vec3 to) {
	return { getRandomFloat(from.x, to.x),getRandomFloat(from.y, to.y),getRandomFloat(from.z, to.z) };
}
void ParticleSystem::generateParticle()
{
	bool customLifespan = (lifeSpanRange[0] != 0 || lifeSpanRange[1] != 0);
	bool customAlpha = (alpha[0] != 0 || alpha[1] != 0);
	bool customSize = (size[0] != 0 || size[1] != 0);
	bool customPos = vec3HasValue(posRange[0]) || vec3HasValue(posRange[1]);
	Vec3 prePos = { 0,0,0 };
	bool customVel = vec3HasValue(vel[0]) || vec3HasValue(vel[1]);
	Vec3 preVel = { 0,0,0 };
	bool customAcc = vec3HasValue(acc[0]) || vec3HasValue(acc[1]);
	Vec3 preAcc = { 0,0,0 };
	bool customColor = vec3HasValue(color[0]) || vec3HasValue(color[1]);
	Vec3 preColor = { 0,0,0 };

	Particle* p = new Particle(
		customLifespan ? getRandomFloat(lifeSpanRange[0], lifeSpanRange[1]) : 1.0f,
		customPos ? getRandomVec3(posRange[0], posRange[1]) : prePos,
		customVel ? getRandomVec3(vel[0], vel[1]) : preVel,
		customAcc ? getRandomVec3(acc[0], acc[1]) : preAcc,
		customColor ? getRandomVec3(color[0], color[1]) : preColor,
		customColor ? getRandomVec3(color[0], color[1]) : preColor,
		customAlpha ? alpha[0] : 1.0f,
		customAlpha ? alpha[1] : 1.0f,
		customSize ? getRandomFloat(size[0], size[1]) : 1.0f,
		customSize ? getRandomFloat(size[0], size[1]) : 1.0f
	);

	particles.push_back(p);
}

bool ParticleSystem::shouldDissapear()
{
	return age > lifeSpan && particles.empty();
}
