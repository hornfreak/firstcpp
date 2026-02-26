#pragma once

#include <cmath>
struct Vector3 {
  double x, y, z;

  // Operator overloading: Allows "vecA + vecB"
  Vector3 operator+(const Vector3 &other) const {
    return {x + other.x, y + other.y, z + other.z};
  }

  // Subtraction operator for realitve position maths
  Vector3 operator-(const Vector3 &other) const {
    return {x - other.x, y - other.y, z - other.z};
  }

  // Allows "vec * scalar" (Scaling a vector)
  Vector3 operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  // Compound assignment: "vecA += vecB"
  Vector3 operator+=(const Vector3 &other) {
    return {x += other.x, y += other.y, z += other.z};
  }

  // Dot product: Measures how much two vectors point in the same direction
  double dot(const Vector3 &other) const {
    return (x * other.x) + (y * other.y) + (z * other.z);
  }

  // Length (Magnitude): The distance of the vector
  double length() const { return std::sqrt(x * x + y * y + z * z); }

  // Normalization: Returns a unit vector (length of 1.0)
  Vector3 normalized() const {
    double l = length();
    if (l > 0.0001) {
      return {x / l, y / l, z / l};
    }
    return {0, 0, 0};
  }

  // Cross Product: finding 3rd axis for something
  Vector3 cross(const Vector3 &other) const {
    return {(y * other.z) - (z * other.y), (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x)};
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
