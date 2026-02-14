#include "physics_engine.h"
#include <fmt/core.h>
#include <chrono>
#include <thread>

int main() {
    fmt::print("DEBUG: Program has started\n");
    // Start at 100m, 0 velocity, -9,8 acceleration
    Particle ball(100.0,0.0,-9.8);

    // Get the starting time point
    auto lastTime = std::chrono::high_resolution_clock::now();

    fmt::print("DEBUG: Entering loop...\n");
    fmt::print("{:<10} | {:<10} | {:<10} | {:<10}\n", "Step", "Time", "Pos", "Vel");
    fmt::print("--------------------------------------------\n");

    for(int i = 0; i < 100; ++i) {
        // fmt::print("DEBUG: Inside loop iteration {}\n", i);
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        double dt = elapsed.count();
        lastTime = currentTime;

        ball.update(dt);

        // Check for the ground
        ball.handleCollision(0.8);

        fmt::print("Step {:>2}: Pos {:>7.2f}m | Vel {:>7.2f}m/s\n", i, ball.getPosition(), ball.getVelocity());

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    fmt::print("DEBUG: Program ended.\n");
    return 0;
}
