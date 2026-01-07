#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(AcquisitionController* controller, QWidget* parent)
    : QMainWindow(parent), m_controller(controller)
{
    setupUi();

    connect(m_controller, &AcquisitionController::measurementsReady,
            this, &MainWindow::onMeasurements);

    connect(m_controller, &AcquisitionController::syncStatusChanged,
            this, &MainWindow::onSyncStatus);
}

void MainWindow::setupUi()
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // --- LIVE SENSORS ---
    QWidget* liveWidget = new QWidget(this);
    QVBoxLayout* liveLayout = new QVBoxLayout(liveWidget);

    m_liveTable = new QTableWidget(0, 2, this);
    m_liveTable->setHorizontalHeaderLabels({"Sensor", "Value"});
    m_liveTable->horizontalHeader()->setStretchLastSection(true);

    liveLayout->addWidget(new QLabel("Live sensors:", this));
    liveLayout->addWidget(m_liveTable);

    // --- STACKED WIDGET (History <-> Diagram) ---
    m_stack = new QStackedWidget(this);

    // HISTORY
    m_historyWidget = new QWidget(this);
    QVBoxLayout* histLayout = new QVBoxLayout(m_historyWidget);

    m_historyTable = new QTableWidget(0, 3, this);
    m_historyTable->setHorizontalHeaderLabels({"Sensor", "Value", "Timestamp"});
    m_historyTable->horizontalHeader()->setStretchLastSection(true);

    histLayout->addWidget(new QLabel("History:", this));
    histLayout->addWidget(m_historyTable);

    // DIAGRAM
    m_diagramWidget = new QWidget(this);
    QVBoxLayout* diagLayout = new QVBoxLayout(m_diagramWidget);

    m_chart = new QChart();
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    diagLayout->addWidget(m_chartView);

    m_stack->addWidget(m_historyWidget);   // index 0
    m_stack->addWidget(m_diagramWidget);   // index 1

    // --- BUTTONS ---
    m_startStopBtn = new QPushButton("Start", this);
    m_syncBtn = new QPushButton("Sync now", this);
    m_switchBtn = new QPushButton("Switch to Diagram", this);

    connect(m_startStopBtn, &QPushButton::clicked,
            this, &MainWindow::onStartStop);

    connect(m_syncBtn, &QPushButton::clicked,
            this, &MainWindow::onSyncNow);

    connect(m_switchBtn, &QPushButton::clicked,
            this, &MainWindow::onSwitchView);

    m_statusLabel = new QLabel("Acquisition: OFF", this);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_startStopBtn);
    btnLayout->addWidget(m_syncBtn);
    btnLayout->addWidget(m_switchBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_statusLabel);

    // --- FINAL LAYOUT ---
    mainLayout->addWidget(liveWidget);
    mainLayout->addWidget(m_stack);
    mainLayout->addLayout(btnLayout);

    setCentralWidget(central);
    setWindowTitle("Embedded Data Logger");
}

void MainWindow::onMeasurements(const std::vector<Measurement>& m)
{
    updateLiveValues(m);
    refreshHistory();
    refreshDiagram();
}

void MainWindow::updateLiveValues(const std::vector<Measurement>& m)
{
    m_liveTable->setRowCount(m.size());

    for (int i = 0; i < (int)m.size(); i++) {
        m_liveTable->setItem(i, 0,
                             new QTableWidgetItem(QString::fromStdString(m[i].sensorName)));

        m_liveTable->setItem(i, 1,
                             new QTableWidgetItem(QString::number(m[i].value)));
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

        long long ts =
            std::chrono::duration_cast<std::chrono::seconds>(
                last[i].timestamp.time_since_epoch()
                ).count();

        m_historyTable->setItem(i, 2,
                                new QTableWidgetItem(QString::number(ts)));
    }
}

void MainWindow::refreshDiagram()
{
    auto last = m_controller->getLastMeasurements(20);

    m_chart->removeAllSeries();

    // Regrouper par capteur
    std::map<std::string, QLineSeries*> seriesMap;

    for (const auto& m : last) {
        if (!seriesMap.count(m.sensorName))
            seriesMap[m.sensorName] = new QLineSeries();

        long long ts =
            std::chrono::duration_cast<std::chrono::seconds>(
                m.timestamp.time_since_epoch()
                ).count();

        seriesMap[m.sensorName]->append(ts, m.value);
    }

    // Ajouter les séries au graphique
    for (auto& kv : seriesMap) {
        kv.second->setName(QString::fromStdString(kv.first));
        m_chart->addSeries(kv.second);
    }

    QValueAxis* axisX = new QValueAxis();
    axisX->setTitleText("Timestamp");

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Value");
    axisY->setRange(18, 60);

    m_chart->setAxisX(axisX);
    m_chart->setAxisY(axisY);


    for (auto& kv : seriesMap) {
        kv.second->attachAxis(axisX);
        kv.second->attachAxis(axisY);
    }

    m_chart->setTitle("Sensor evolution (last 10 points per sensor)");
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
    m_statusLabel->setText("Sync requested...");

    auto last = m_controller->getLastMeasurements(20);
    m_controller->manualSync(last);
}

void MainWindow::onSyncStatus(bool ok, const QString& message)
{
    if (ok)
        m_statusLabel->setText("Sync OK");
    else
        m_statusLabel->setText("Sync ERROR: " + message);
}

void MainWindow::onSwitchView()
{
    if (m_stack->currentIndex() == 0) {
        m_stack->setCurrentIndex(1);
        m_switchBtn->setText("Switch to History");
    } else {
        m_stack->setCurrentIndex(0);
        m_switchBtn->setText("Switch to Diagram");
    }
}

