#pragma once

struct Vector3 {
  double x, y, z;

  // Operator overloading: Allows "vecA + vecB"
  Vector3 operator+(const Vector3 &other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  // Allows "vec * scalar" (Scaling a vector)
  Vector3 operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  // Compound assignment: "vecA += vecB"
  Vector3 operator+=(const Vector3 &other) {
    return {x += other.x, y += other.y, z += other.z};
  }
};

class Particle {
public:
  // Now we use Vector2 for position & velocity!
  Particle(Vector3 startPos, Vector3 startVel, double grav);

  // The core of the simulation: Update state based on time
  void update(double deltaTime, Vector3 externalForce);
  void handleCollision(double elasticity);

  // Getter for 2D vectors
  Vector3 getPosition() const { return position; }
  Vector3 getVelocity() const { return velocity; }

private:
  Vector3 position;
  Vector3 velocity;
  double gravity;               // The constant acceleration
  double dragCoefficient = 0.1; // A good starting value for aire resistance
};
