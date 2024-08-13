#pragma onces
#include <chrono>
using namespace std;
using namespace chrono;

class Timer {
private:
    high_resolution_clock::time_point start_time;
    high_resolution_clock::time_point pause_time;
    high_resolution_clock::time_point stop_time;
    bool paused = false;
    bool running = false;
    bool stopped = false;

public:
    void start() {
        start_time = high_resolution_clock::now();
        running = true;
        paused = false;
        stopped = false;
    }

    void pause() {
        if (running && !paused) {
            pause_time = high_resolution_clock::now();
            paused = true;
        }
    }

    void resume() {
        if (running && paused) {
            start_time += high_resolution_clock::now() - pause_time;
            paused = false;
        }
    }

    void stop() {
        if (running && !stopped) {
            stop_time = high_resolution_clock::now();
            running = false;
            stopped = true;
        }
    }

    int getElapsedTime() const {
        if (running) {
            if (paused) {
                return duration_cast<seconds>(pause_time - start_time).count();
            } else {
                return duration_cast<seconds>(high_resolution_clock::now() - start_time).count();
            }
        } else if (stopped) {
            return duration_cast<seconds>(stop_time - start_time).count();
        }
        return 0;
    }
};
