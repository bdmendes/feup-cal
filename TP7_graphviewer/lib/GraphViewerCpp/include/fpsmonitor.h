#ifndef FPSMONITOR_H_INCLUDED
#define FPSMONITOR_H_INCLUDED

#include <chrono>
#include <queue>

/**
 * @brief Class to monitor number of frames per second.
 */
class FPSMonitor {
private:
    /**
     * @brief Queue with all frame timestamps in the past Dt interval
     */
    std::queue<std::chrono::high_resolution_clock::time_point> q;
    /**
     * @brief Time interval to save frame timestamps
     */
    std::chrono::high_resolution_clock::duration Dt;
public:
    /**
     * @brief Construct a new FPSMonitor object
     * 
     * You can specify the time interval (in milliseconds) the FPSMonitor uses
     * to determine the number of frames per second.
     * 
     * If the interval is longer, it takes more time to stabilize at the
     * beginning and when there is a change in actual FPS, but the reading is
     * more exact once it stabilizes.
     * 
     * If the interval is shorter, readings are not as exact but they stabilize
     * faster.
     * 
     * We advise you use an interval around 1000ms.
     * 
     * @param ms Time interval in milliseconds
     */
    FPSMonitor(int ms);

    /**
     * @brief Set the time interval
     * 
     * @param ms Time interval in milliseconds
     */
    void setInterval(int ms);

    /**
     * @brief Count a frame.
     */
    void count();

    /**
     * @brief Get number of frames per second over the specified interval.
     * 
     * @return Frames Per Second
     */
    float getFPS() const;
};

#endif // FPSMONITOR_H_INCLUDED
