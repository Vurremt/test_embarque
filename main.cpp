#include <QCoreApplication>
#include <QDebug>

#include "core/AcquisitionController.h"
#include "core/TemperatureSensor.h"
#include "core/HumiditySensor.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    AcquisitionController acquisitionController;
    acquisitionController.addSensor(std::make_unique<TemperatureSensor>());
    acquisitionController.addSensor(std::make_unique<HumiditySensor>());

    QObject::connect(&acquisitionController, &AcquisitionController::measurementsReady,
                    [](const std::vector<Measurement>& measurements){
                        qDebug() << "New measurements:";
                        for(const auto& m : measurements){
                            qDebug() << QString::fromStdString(m.sensorName) << m.value;
                        }
                    }
    );

    acquisitionController.start();

    return app.exec();
}
