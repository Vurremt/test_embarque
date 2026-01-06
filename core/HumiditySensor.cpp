#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(): SensorBase("Humidity") {}

double HumiditySensor::readValue(){
    std::uniform_real_distribution<double> dist(40.0, 60.0);
    return dist(m_rng);
}
