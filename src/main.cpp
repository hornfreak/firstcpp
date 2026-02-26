#include "physics_engine.h"
#include "user.h"
#include <chrono>
#include <fmt/core.h>
#include <fstream>
#include <thread>

int main() {
  // 1. Setup Simulation Objects
  Particle ball({0.0, 50.0, 10.0}, {2.0, 5.0, 0.0}, -9.8);
  User hmd({0.0, 1.8, 0.0}); // User at origin, standing 1.8m tall

  // 2. Setup Data Logging
  std::ofstream outFile("../simulation_data.csv");
  if (outFile.is_open()) {
    outFile << "step,time,pos_x,pos_y,pos_z,local_x,local_y,local_z\n";
  }

  // 3. Simulation Variables
  double accumulator = 0.0;
  const double dt = 0.01; // 100Hz Fixed Timestep
  double totalSimTime = 0.0;
  int stepCount = 0;

  auto lastTime = std::chrono::high_resolution_clock::now();
  Vector3 wind = {0.0, 0.0, 0.0};

  fmt::print("XR Simulation Started (M5 Mac)...\n");

  while (totalSimTime < 5.0) { // Run for 5 seconds
    auto currentTime = std::chrono::high_resolution_clock::now();
    double frameTime =
        std::chrono::duration<double>(currentTime - lastTime).count();
    lastTime = currentTime;

    if (frameTime > 0.25)
      frameTime = 0.25; // Prevent spiral of death
    accumulator += frameTime;

    // --- FIXED PHYSICS LOOP ---
    while (accumulator >= dt) {
      ball.update(dt, wind);
      ball.handleCollision(0.75);

      // Calculate Local Position for the User
      Vector3 localBall = hmd.getLocalPosition(ball.getPosition());
      bool visible = hmd.isVisible(ball.getPosition(), 90.0);

      if (totalSimTime > 2.0 && totalSimTime < 3.0) {
        // Move the user to the right at 1.5 meters per second
        Vector3 right = hmd.getRightVector();
        Vector3 currentPos = hmd.getPosition();

        // Position = Position + (Direction * speed * deltaTime)
        hmd.setPosition(currentPos + (right * 1.5 * dt));
      }

      // Log Data
      if (outFile.is_open()) {
        Vector3 p = ball.getPosition();
        outFile << fmt::format(
            "{},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f},{:.4f}\n", stepCount,
            totalSimTime, p.x, p.y, p.z, localBall.x, localBall.y, localBall.z);
      }

      // Console HUD Output
      if (stepCount % 20 == 0) {
        fmt::print("\rTime: {:.2f}s | Visible: {:<3} | Local Pos: ({:>5.1f}, "
                   "{:>5.1f}, {:>5.1f})",
                   totalSimTime, visible ? "YES" : "NO", localBall.x,
                   localBall.y, localBall.z);
        std::fflush(stdout);
      }

      accumulator -= dt;
      totalSimTime += dt;
      stepCount++;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  if (outFile.is_open())
    outFile.close();
  fmt::print("\nSimulation complete. Data saved to simulation_data.csv\n");

  return 0;
}
