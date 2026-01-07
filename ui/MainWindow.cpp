#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

MainWindow::MainWindow(AcquisitionController* controller, QWidget* parent)
    : QMainWindow(parent), m_controller(controller)
{
    setupUi();

    connect(m_controller, &AcquisitionController::measurementsReady,
            this, &MainWindow::onMeasurements);
}

void MainWindow::setupUi()
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);

    // --- LIVE TABLE ---
    m_liveTable = new QTableWidget(0, 2, this);
    m_liveTable->setHorizontalHeaderLabels({"Sensor", "Value"});
    m_liveTable->horizontalHeader()->setStretchLastSection(true);

    // --- HISTORY TABLE ---
    m_historyTable = new QTableWidget(0, 3, this);
    m_historyTable->setHorizontalHeaderLabels({"Sensor", "Value", "Timestamp"});
    m_historyTable->horizontalHeader()->setStretchLastSection(true);

    // --- BUTTONS ---
    m_startStopBtn = new QPushButton("Start", this);
    m_syncBtn = new QPushButton("Sync now", this);

    connect(m_startStopBtn, &QPushButton::clicked,
            this, &MainWindow::onStartStop);

    connect(m_syncBtn, &QPushButton::clicked,
            this, &MainWindow::onSyncNow);

    // --- STATUS ---
    m_statusLabel = new QLabel("Acquisition: OFF", this);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_startStopBtn);
    btnLayout->addWidget(m_syncBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_statusLabel);

    // --- FINAL LAYOUT ---
    layout->addWidget(new QLabel("Live sensors:", this));
    layout->addWidget(m_liveTable);

    layout->addWidget(new QLabel("History:", this));
    layout->addWidget(m_historyTable);

    layout->addLayout(btnLayout);

    setCentralWidget(central);
    setWindowTitle("Embedded Data Logger");
}

void MainWindow::onMeasurements(const std::vector<Measurement>& m)
{
    updateLiveValues(m);
    refreshHistory();
}

void MainWindow::updateLiveValues(const std::vector<Measurement>& m)
{
    m_liveTable->setRowCount(m.size());

    for (int i = 0; i < (int)m.size(); i++) {
        m_liveTable->setItem(i, 0, new QTableWidgetItem(
                                       QString::fromStdString(m[i].sensorName)));

        m_liveTable->setItem(i, 1, new QTableWidgetItem(
                                       QString::number(m[i].value)));
    }
}

void MainWindow::refreshHistory()
{
    auto last = m_controller->getLastMeasurements(20);

    m_historyTable->setRowCount(last.size());

    for (int i = 0; i < (int)last.size(); i++) {
        m_historyTable->setItem(i, 0,
                                new QTableWidgetItem(QString::fromStdString(last[i].sensorName)));

        m_historyTable->setItem(i, 1,
                                new QTableWidgetItem(QString::number(last[i].value)));

        m_historyTable->setItem(i, 2,
                                new QTableWidgetItem(QString::number(
                                    std::chrono::duration_cast<std::chrono::seconds>(
                                        last[i].timestamp.time_since_epoch()
                                        ).count()
                                    )));
    }
}

void MainWindow::onStartStop()
{
    if (!m_running) {
        m_controller->start();
        m_startStopBtn->setText("Stop");
        m_statusLabel->setText("Acquisition: ON");
    } else {
        m_controller->stop();
        m_startStopBtn->setText("Start");
        m_statusLabel->setText("Acquisition: OFF");
    }

    m_running = !m_running;
}

void MainWindow::onSyncNow()
{
    //TODO
    m_statusLabel->setText("Sync requested...");
}
