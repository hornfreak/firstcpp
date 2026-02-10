#include "physics_engine.h"

Particle::Particle(double startPos, double startVel, double gravityAmt) : position(startPos), velocity(startVel), gravity(gravityAmt) {}

void Particle::update(double deltaTime) {
    // 1. Acceleration changes velocity : v = v + (a * dt)
    velocity += gravity * deltaTime;

    // 2. velcity changes position: p = p + (v * dt)
    position += velocity * deltaTime;
}

double Particle::getPosition() const { return position;}
double Particle::getVelocity() const { return velocity;}
