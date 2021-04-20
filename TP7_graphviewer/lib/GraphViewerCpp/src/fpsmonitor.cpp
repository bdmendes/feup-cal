#include "fpsmonitor.h"

FPSMonitor::FPSMonitor(int ms):
    Dt(std::chrono::milliseconds(ms))
{
}

void FPSMonitor::setInterval(int ms){
    Dt = std::chrono::milliseconds(ms);
}

void FPSMonitor::count(){
    auto now = std::chrono::high_resolution_clock::now();
    q.push(now);
    while(now-q.front() > Dt){
        q.pop();
    }
}

float FPSMonitor::getFPS() const{
    return float(q.size())/(std::chrono::duration_cast<std::chrono::milliseconds>(Dt).count()/1000.0);
}
