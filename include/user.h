#pragma once

#include "physics_engine.h" // to get access to Vector3
#include <cmath>

struct EularAngles {
  double yaw;   // Left/Right direction
  double pitch; // Up/Down direction
};

class User {
public:
  User(Vector3 startPos) : position(startPos), lookDireciton({0, 0, -1}) {};

  // --- Getters / Setters ---
  void setPosition(Vector3 p) { position = p; }
  Vector3 getPosition() const { return position; }

  // The "Look Vector" from the user to a target point in the world
  Vector3 calculateLookAtTarget(Vector3 target) const {
    Vector3 diff = {target.x - position.x, target.y - position.y,
                    target.z - position.z};

    // Normalize the Vector (make it's length 1.0)
    double distance =
        std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    if (distance > 0.0001) {
      return {diff.x / distance, diff.y / distance, diff.z / distance};
    }
    return {0, 0, 0};
  }

  EularAngles getAnglesToTarget(Vector3 target) const {
    Vector3 diff = {target.x - this->position.x, target.y - this->position.y,
                    target.z - this->position.z};

    // Yaw: atan2 handles X & Z plane
    double yaw = std::atan2(diff.x, diff.z) * (180.0 / M_PI);

    // Pitch: angle relative to the horizon
    double groundDist = std::sqrt(diff.x * diff.x + diff.z * diff.z);
    double pitch = std::atan2(diff.y, groundDist) * (180.0 / M_PI);

    return {yaw, pitch};
  }

  bool isVisible(Vector3 target, double fovDegrees) const {
    // 1. Get the direction vector to the target
    Vector3 toTarget = calculateLookAtTarget(target);

    // 2. Dot Product: (dirA.x * dirB.x) + (dirA.y * dirB.y) + (dirA.z * dirB.z)
    // lookDirection should be normalized (e.g. {0,0,-1} for forward)
    double dot = (lookDireciton.x * toTarget.x) +
                 (lookDireciton.y * toTarget.y) +
                 (lookDireciton.z * toTarget.z);

    // 3. Convert FOV to a threshold
    // A 90 degree FOV menas 45 degrees to the left & 45 to the right
    double threshold = std::cos((fovDegrees / 2.0) * (M_PI / 180.0));

    // If the dot product is greater than the threshold, it is in the Conve
    return dot >= threshold;
  }

  void updateLookDirection(double yawDegrees, double pitchDegrees) {
    // Convert degrees to radians for math functions
    double yawRad = yawDegrees * (M_PI / 180.0);
    double pitchRad = pitchDegrees * (M_PI / 180.0);

    // Standard Spherical to Cartisan conversion
    // NOTE:: this assumes Z-foreard, Y-up
    lookDireciton.x = std::sin(yawRad) * std::cos(pitchRad);
    lookDireciton.y = std::sin(pitchRad);
    lookDireciton.z = std::cos(yawRad) * std::cos(pitchRad);

    // The lookDirection is now a unit vector pointing exactly where the user is
    // looking
  }

private:
  Vector3 position;
  Vector3 lookDireciton;
};
