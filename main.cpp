#include <QCoreApplication>
#include <QDebug>

#include "core/SensorManager.h"
#include "core/TemperatureSensor.h"
#include "core/HumiditySensor.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SensorManager manager;
    manager.addSensor(std::make_unique<TemperatureSensor>());
    manager.addSensor(std::make_unique<HumiditySensor>());

    auto measurements = manager.readAll();
    for (const auto& m : measurements) {
        qDebug() << QString::fromStdString(m.sensorName)
        << m.value;
    }

    return 0;
}
