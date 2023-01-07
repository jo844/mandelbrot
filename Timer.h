#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

class Timer
{
    public:
        Timer(int i)
        {
            start = std::chrono::high_resolution_clock::now();
            zoom = i;
        }

        ~Timer()
        {
            Stop();
        }

        void Stop()
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto st = std::chrono::time_point_cast<std::chrono::microseconds>(start).time_since_epoch().count();
            auto en = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();

            auto duration = en - st;

            double s = duration * 0.000001;
            std::cout << "Zoom " << zoom << " done in " << s << "seconds\n";
        }

    private:
        int zoom;
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#endif