#include "SensorManager.h"

SensorManager::SensorManager() : m_sensors() {}

void SensorManager::addSensor(std::unique_ptr<ISensor> sensor) {
    m_sensors.push_back(std::move(sensor));
}

std::vector<Measurement> SensorManager::readAll() {
    std::vector<Measurement> measurement_list;
    measurement_list.reserve(m_sensors.size());

    for(auto& it : m_sensors){
        Measurement measurement = Measurement{it->name(), it->readValue(), std::chrono::system_clock::now()};
        measurement_list.push_back(std::move(measurement));
    }

    return measurement_list;
}
