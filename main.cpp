#include <QApplication>
#include <QDebug>

#include "ui/mainwindow.h"

#include "core/AcquisitionController.h"
#include "core/TemperatureSensor.h"
#include "core/HumiditySensor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AcquisitionController controller;
    controller.addSensor(std::make_unique<TemperatureSensor>());
    controller.addSensor(std::make_unique<HumiditySensor>());

    MainWindow mainWindow(&controller);
    mainWindow.show();

    return app.exec();
}
