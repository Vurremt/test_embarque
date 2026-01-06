#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "SensorBase.h"

class TemperatureSensor : public SensorBase
{
public:
    TemperatureSensor();

    double readValue() override;
};

#endif // TEMPERATURESENSOR_H
