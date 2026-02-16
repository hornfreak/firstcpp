#include "physics_engine.h"
#include "user.h"
#include <chrono>
#include <cstdio>
#include <fmt/core.h>
#include <fstream>
#include <thread>

int main() {
  Particle ball({0.0, 100.0, 0.0}, {2.0, 0.0, 5.0}, -9.81);
  User hmd({0, 1.65, 0}); // user standing at origion at my height (in meters)

  std::ofstream outFile("../simulation_data.csv");
  if (outFile.is_open()) {
    outFile << "step,time,pos_x,pos_y,pos_z\n";
  }

  // --- Fixed Timestep Variables ---
  double accumulator = 0.0;
  const double dt = 0.01; // 100Hz physics (10ms steps)
  double totalSimTime = 0.0;
  int stepCount = 0;

  auto lastTime = std::chrono::high_resolution_clock::now();
  Vector3 wind = {0.5, 0.0, 0.2};

  fmt::print("Starting Fixed Timestep Simulation\n");

  // Run for 10 seconds of "simulated" time
  while (totalSimTime < 10.0) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    double frameTime =
        std::chrono::duration<double>(currentTime - lastTime).count();
    lastTime = currentTime;

    // Cap frameTime to avoid "spiral of death" if the app hangs
    if (frameTime > 0.25)
      frameTime = 0.25;

    accumulator += frameTime;

    // --- THE FIXED LOOP ---
    // As long as we have enough "time" in the bucket, run physics
    while (accumulator >= dt) {
      ball.update(dt, wind);
      ball.handleCollision(0.8);

      totalSimTime += dt;
      stepCount++;

      // Log every physics step for greater precision
      if (outFile.is_open()) {
        Vector3 p = ball.getPosition();
        outFile << fmt::format("{},{:.4f},{:.4f},{:.4f},{:.4f}\n", stepCount,
                               totalSimTime, p.x, p.y, p.z);
      }
      accumulator -= dt;
    }

    Vector3 lookVec = hmd.calculateLookAtTarget(ball.getPosition());
    EularAngles angles = hmd.getAnglesToTarget(ball.getPosition());

    // Print to the console less frequently so it doesn't log the Mac
    if (stepCount % 10 == 0) {
      bool visible = hmd.isVisible(ball.getPosition(), 90.0);
      EularAngles toBall = hmd.getAnglesToTarget(ball.getPosition());

      fmt::print("User needs to look: Yaw {:.1f}°, Pitch {:.1f}° to see ball\n",
                 angles.yaw, angles.pitch);
      // Clear the line & print a HUD
      fmt::print(
          "\r[HUD] Visible: {:<5} | To Ball: Yaw {:>6.1f}°, Pitch: {:>6.1f}°",
          visible ? "YES" : "NO", toBall.yaw, toBall.pitch);
      std::fflush(stdout); // Forces the console to overwrite the same line
    }

    // Slow down the "Visual" loop to let the CPU breathe
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  if (outFile.is_open())
    outFile.close();
}
