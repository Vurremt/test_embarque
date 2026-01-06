#ifndef SENSORBASE_H
#define SENSORBASE_H

#include "ISensor.h"
#include <random>

class SensorBase : public ISensor
{
public:
    SensorBase(std::string name);
    virtual ~SensorBase() = default;

    std::string name() const override;

protected:
    std::string m_name;
    std::default_random_engine m_rng;
};

#endif // SENSORBASE_H
