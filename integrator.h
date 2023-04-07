#pragma once
#ifndef INTEGRATOR
#define INTEGRATOR

#include "particle.h"
#include "forcefield.h"

class Integrator {

public:

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() = 0;

private:

};

class MidpointIntegrator : public Integrator {

public:

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) {

		float m = particle->getMass();
		Vec3f v = particle->getVelocity();
		Vec3f p = particle->getPosition();

		Vec3f mid_v;
		Vec3f mid_p;
		mid_v = v + forcefield->getAcceleration(p, m, t) * dt * 0.5f;
		mid_p = p + v * dt * 0.5f;

		Vec3f new_v;
		Vec3f new_p;
		new_v = v + forcefield->getAcceleration(mid_p, m, t + dt * 0.5f) * dt;
		new_p = p + mid_v * dt;

		particle->setVelocity(new_v);
		particle->setPosition(new_p);
		particle->increaseAge(dt);

	}

	virtual Vec3f getColor() { return Vec3f(1.0f, 0, 0); }

private:

};

class EulerIntegrator : public Integrator {

public:

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) {

		float m = particle->getMass();
		Vec3f v = particle->getVelocity();
		Vec3f p = particle->getPosition();

		Vec3f new_v;
		Vec3f new_p;
		new_v = v + forcefield->getAcceleration(p, m, t) * dt;
		new_p = p + v * dt;

		particle->setVelocity(new_v);
		particle->setPosition(new_p);
		particle->increaseAge(dt);

	}
	virtual Vec3f getColor() { return Vec3f(0, 1.0f, 0); }

private:

};

class RungeKuttaIntegrator : public Integrator {

	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) {

		float m = particle->getMass();
		Vec3f v = particle->getVelocity();
		Vec3f p = particle->getPosition();

		Vec3f p_k1 = p;
		Vec3f a_k1 = forcefield->getAcceleration(p_k1, m, t);
		Vec3f v_k1 = v;

		Vec3f p_k2 = p + v_k1 * dt * 0.5f;
		Vec3f a_k2 = forcefield->getAcceleration(p_k2, m, t + dt * 0.5f);
		Vec3f v_k2 = v + a_k1 * dt * 0.5f;

		Vec3f p_k3 = p + v_k2 * dt * 0.5f;
		Vec3f a_k3 = forcefield->getAcceleration(p_k3, m, t + dt * 0.5f);
		Vec3f v_k3 = v + a_k2 * dt * 0.5f;

		Vec3f p_k4 = p + v_k3 * dt;
		Vec3f a_k4 = forcefield->getAcceleration(p_k4, m, t + dt);
		Vec3f v_k4 = v + a_k3 * dt;
		
		Vec3f new_v = v + (a_k1 + 2 * a_k2 + 2 * a_k3 + a_k4) * (dt / 6.0f);
		Vec3f new_p = p + (v_k1 + 2 * v_k2 + 2 * v_k3 + v_k4) * (dt / 6.0f);

		particle->setVelocity(new_v);
		particle->setPosition(new_p);
		particle->increaseAge(dt);

	}

	virtual Vec3f getColor() { return Vec3f(0, 0, 1.0f); }

};

#endif // !INTEGRATOR
