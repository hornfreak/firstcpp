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
    // determine look at which is just VecA - VecB
    Vector3 diff = target - position;

    // Yaw: atan2 handles X & Z plane
    double yaw = std::atan2(diff.x, diff.z) * (180.0 / M_PI);

    // Pitch: angle relative to the horizon
    double groundDist = std::sqrt(diff.x * diff.x + diff.z * diff.z);
    double pitch = std::atan2(diff.y, groundDist) * (180.0 / M_PI);

    return {yaw, pitch};
  }

  bool isVisible(Vector3 target, double fovDegrees) const {
    // 1. Get the direction vector to the target
    Vector3 toTarget = (target - position).normalized();

    // 2. Use the dot product method
    double dotproduct = lookDireciton.dot(toTarget);

    // 3. Convert FOV to a threshold
    // A 90 degree FOV menas 45 degrees to the left & 45 to the right
    double threshold = std::cos((fovDegrees / 2.0) * (M_PI / 180.0));

    // If the dot product is greater than the threshold, it is in the Conve
    return dotproduct >= threshold;
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

  Vector3 getLocalPosition(Vector3 targetWorldPos) {
    // 1. Translate: Find the world-space offset
    Vector3 relative = {targetWorldPos.x - position.x,
                        targetWorldPos.y - position.y,
                        targetWorldPos.z - position.z};

    // 2. Rotate: This is simplied 2D rotation (Yaw only)
    // to align the world to the user's horizontal heading.
    double angle = std::atan2(lookDireciton.x, lookDireciton.z);
    double s = std::sin(-angle);
    double c = std::cos(-angle);

    Vector3 local;
    local.x = relative.x * c - relative.z * s;
    local.z = relative.x * s + relative.z * c;
    local.y = relative.y; // Height remains the same for simple yaw

    return local;
  }

  Vector3 getRightVector() const {
    Vector3 worldUp = {0.0, 1.0, 0.0};

    // Cross Product: Forward x WorldUp = Right
    // This returns a vector pointing 90 degrees to the user's right
    return lookDireciton.cross(worldUp).normalized();
  }

private:
  Vector3 position;
  Vector3 lookDireciton;
};
