#include <chrono>
#include <thread>

class Timer {
public:
    Timer() : startTime(std::chrono::high_resolution_clock::now()) {}

    // Method to get the elapsed time in seconds
    double getElapsedSeconds() {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count();
        return static_cast<double>(deltaTime) / 1e6; // Convert microseconds to seconds
    }

    // Method to reset the timer
    void reset() {
        startTime = std::chrono::high_resolution_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};