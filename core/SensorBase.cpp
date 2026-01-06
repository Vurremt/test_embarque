#include "SensorBase.h"

SensorBase::SensorBase(std::string name): m_name(std::move(name)) {
    std::random_device rd;
    m_rng.seed(rd());
}

std::string SensorBase::name() const {
    return m_name;
}
