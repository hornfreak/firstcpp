#include "physics_engine.h"
#include <fmt/core.h>
#include <chrono>
#include <thread>

int main() {
    // Start at 100m, 0 velocity, -9,8 acceleration
    Particle ball(100.0,0.0,-9.8);

    // Get the starting time point
    auto lastTime = std::chrono::high_resolution_clock::now();

    fmt::print("{:<10} | {:<10} | {:<10} | {:<10}\n", "Step", "Time", "Pos", "Vel");
    fmt::print("--------------------------------------------\n");

    for(int i = 0; i < 50; ++i) {
        // Run for 100 iterations
        auto currentTime = std::chrono::high_resolution_clock::now();

        // Calculate Delta Time in seconds
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        double dt = elapsed.count();
        lastTime = currentTime;

        ball.update(dt);

        fmt::print("{:<10} | {:10.4f} | {:10.2f} | {:10.2f}\n",
                i, i * 0.1, ball.getPosition(), ball.getVelocity());

        // Slow down the loop slightly so we can see it
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Stop if the ball hits the ground
        if(ball.getPosition() <= 0) {
            fmt::print("Collision detected with ground!\n");
        }
    }

    return 0;
}
