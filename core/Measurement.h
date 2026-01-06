#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <string>
#include <chrono>

struct Measurement{
    std::string sensorName;
    double value;
    std::chrono::system_clock::time_point timestamp;
};

#endif // MEASUREMENT_H
