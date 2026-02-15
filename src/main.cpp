#include "physics_engine.h"
#include <chrono>
#include <fmt/core.h>
#include <fstream>
#include <thread>

int main() {
  Particle ball({0.0, 100.0, 0.0}, {2.0, 0.0, 5.0}, -9.81);

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

      accumulator -= dt;
      totalSimTime += dt;
      stepCount++;

      // Log every physics step for greater precision
      if (outFile.is_open()) {
        Vector3 p = ball.getPosition();
        outFile << fmt::format("{},{:.4f},{:.4f},{:.4f},{:.4f}\n", stepCount,
                               totalSimTime, p.x, p.y, p.z);
      }
    }

    // Print to the console less frequently so it doesn't log the Mac
    if (stepCount % 10 == 0) {
      fmt::print("Time: {:.2f}s | Pos Y: {:.2f}m\n", totalSimTime,
                 ball.getPosition().y);
    }

    // Slow down the "Visual" loop to let the CPU breathe
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  if (outFile.is_open())
    outFile.close();
}
