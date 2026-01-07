#include "AcquisitionController.h"

AcquisitionController::AcquisitionController(QObject* parent) :
    QObject(parent),
    m_db("data.db"),
    m_sync("https://httpbin.org/post", "MY_API_KEY")
{
    m_db.open();
    m_db.init();

    connect(&m_timer, &QTimer::timeout,
            this, &AcquisitionController::onTimeout);

    connect(&m_syncTimer, &QTimer::timeout,
            this, &AcquisitionController::onSyncTimeout);

    connect(&m_sync, &NetworkSyncManager::syncFinished,
            this, &AcquisitionController::syncStatusChanged);
}

void AcquisitionController::addSensor(std::unique_ptr<ISensor> sensor){
    m_manager.addSensor(std::move(sensor));
}

void AcquisitionController::start()
{
    m_timer.start(m_intervalMs);
    m_syncTimer.start(m_syncIntervalSec * 1000);
}

void AcquisitionController::stop()
{
    m_timer.stop();
    m_syncTimer.stop();
}

void AcquisitionController::setInterval(int ms){
    m_intervalMs = ms;
    if (m_timer.isActive())
        m_timer.start(m_intervalMs);
}

std::vector<Measurement> AcquisitionController::getLastMeasurements(int count){
    return m_db.getLastMeasurements(count);
}

void AcquisitionController::onTimeout(){
    auto measurements = m_manager.readAll();

    for(const auto& m : measurements){
        m_db.insertMeasurement(m);
    }

    emit measurementsReady(measurements);
}

void AcquisitionController::onSyncTimeout()
{
    auto last = m_db.getLastMeasurements(20);
    m_sync.syncMeasurements(last);
}

void AcquisitionController::manualSync(const std::vector<Measurement>& measurements)
{
    m_sync.syncMeasurements(measurements);
}
