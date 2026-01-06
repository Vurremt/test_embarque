#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <memory>
#include <vector>

#include "ISensor.h"
#include "Measurement.h"

class SensorManager
{
public:
    SensorManager();
    void addSensor(std::unique_ptr<ISensor> sensor);
    std::vector<Measurement> readAll();

private:
    std::vector<std::unique_ptr<ISensor>> m_sensors;
};

#endif // SENSORMANAGER_H
