#pragma once

class Particle {
public:
    Particle(double startPos, double startVel, double gravityAmt);

    // The core of the simulation: Update state based on time
    void update(double deltaTime);

    double getPosition() const;
    double getVelocity() const;
    void handleCollision(double elasticity);
    
private:
    double position;
    double velocity;
    double gravity; // The constant acceleration
};
