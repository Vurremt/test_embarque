#ifndef ACQUISITIONCONTROLLER_H
#define ACQUISITIONCONTROLLER_H

#include <QObject>
#include <QTimer>

#include <vector>
#include <memory>

#include "SensorManager.h"
#include "Measurement.h"
#include "DatabaseManager.h"

class AcquisitionController : public QObject
{
    Q_OBJECT
public:
    explicit AcquisitionController(QObject *parent = nullptr);

    void addSensor(std::unique_ptr<ISensor> sensor);
    void start();
    void stop();
    void setInterval(int ms);

signals:
    void measurementsReady(const std::vector<Measurement>& measurements);

private slots:
    void onTimeout();

private:
    SensorManager m_manager;
    QTimer m_timer;
    int m_intervalMs = 1000;

    DatabaseManager m_db;
};

#endif // ACQUISITIONCONTROLLER_H
