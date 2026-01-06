#ifndef ISENSOR_H
#define ISENSOR_H

#include <string>

class ISensor {
public:
    virtual ~ISensor() = default;

    virtual std::string name() const = 0;
    virtual double readValue() = 0;
};

#endif // ISENSOR_H
