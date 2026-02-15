#include "physics_engine.h"
#include <cmath>

Particle::Particle(Vector3 startPos, Vector3 startVel, double grav)
    : position(startPos), velocity(startVel), gravity(grav) {}

void Particle::update(double deltaTime, Vector3 externalForce) {
  // 1. Calculate Drag (opposing velocity)
  // We calculate drag for each component
  Vector3 dragForce = {-dragCoefficient * velocity.x * std::abs(velocity.x),
                       -dragCoefficient * velocity.y * std::abs(velocity.y),
                       -dragCoefficient * velocity.z * std::abs(velocity.z)};

  // 2. Sum our forces (Gravity is only in Y)
  Vector3 totalAcceleration = externalForce + dragForce;
  totalAcceleration.y += gravity;

  // 3. Update Velocity: v = v + a*dt
  velocity += totalAcceleration * deltaTime;

  // 4. Update Position: p = p + v*dt
  position += velocity * deltaTime;
}

void Particle::handleCollision(double elasticity) {
  if (position.y <= 0) {
    position.y = 0;
    velocity.y *= -elasticity;
    velocity.x *= 0.95; // Friction
    velocity.z *= 0.95;
  }
}
