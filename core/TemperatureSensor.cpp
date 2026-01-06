#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(): SensorBase("Temperature") {}

double TemperatureSensor::readValue(){
    std::uniform_real_distribution<double> dist(18.0, 26.0);
    return dist(m_rng);
}
