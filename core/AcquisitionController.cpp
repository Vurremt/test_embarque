#include "AcquisitionController.h"

AcquisitionController::AcquisitionController(QObject *parent)
    : QObject{parent}, m_db("data.db")
{
    m_db.open();
    m_db.init();

    QObject::connect(&m_timer, &QTimer::timeout, this, &AcquisitionController::onTimeout);
}

void AcquisitionController::addSensor(std::unique_ptr<ISensor> sensor){
    m_manager.addSensor(std::move(sensor));
}

void AcquisitionController::start(){
    m_timer.start(m_intervalMs);
}

void AcquisitionController::stop(){
    m_timer.stop();
}

void AcquisitionController::setInterval(int ms){
    m_intervalMs = ms;
    if (m_timer.isActive())
        m_timer.start(m_intervalMs);
}

void AcquisitionController::onTimeout(){
    auto measurements = m_manager.readAll();

    for(const auto& m : measurements){
        m_db.insertMeasurement(m);
    }

    emit measurementsReady(measurements);
}
