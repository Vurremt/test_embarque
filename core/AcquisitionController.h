#ifndef ACQUISITIONCONTROLLER_H
#define ACQUISITIONCONTROLLER_H

#include <QObject>
#include <QTimer>

#include <vector>
#include <memory>

#include "SensorManager.h"
#include "Measurement.h"
#include "DatabaseManager.h"

#include "NetworkSyncManager.h"

class AcquisitionController : public QObject
{
    Q_OBJECT
public:
    explicit AcquisitionController(QObject *parent = nullptr);

    void addSensor(std::unique_ptr<ISensor> sensor);
    void start();
    void stop();
    void setInterval(int ms);
    std::vector<Measurement> getLastMeasurements(int count);
    void manualSync(const std::vector<Measurement>& measurements);

signals:
    void measurementsReady(const std::vector<Measurement>& measurements);
    void syncStatusChanged(bool ok, QString message);

private slots:
    void onTimeout();
    void onSyncTimeout();

private:
    SensorManager m_manager;
    QTimer m_timer;
    int m_intervalMs = 1000;

    DatabaseManager m_db;

    NetworkSyncManager m_sync;
    QTimer m_syncTimer;
    int m_syncIntervalSec = 10;
};

#endif // ACQUISITIONCONTROLLER_H
