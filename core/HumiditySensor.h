#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "SensorBase.h"

class HumiditySensor : public SensorBase
{
public:
    HumiditySensor();

    double readValue() override;
};

#endif // HUMIDITYSENSOR_H
