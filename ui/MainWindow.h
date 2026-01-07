#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <QStackedWidget>

#include "AcquisitionController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(AcquisitionController* controller, QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void setupUi();
    void updateLiveValues(const std::vector<Measurement>& m);
    void refreshHistory();
    void refreshDiagram();

private slots:
    void onMeasurements(const std::vector<Measurement>& m);
    void onStartStop();
    void onSyncNow();
    void onSyncStatus(bool ok, const QString& message);
    void onSwitchView();

private:
    AcquisitionController* m_controller;

    QTableWidget* m_liveTable;
    QTableWidget* m_historyTable;

    QPushButton* m_startStopBtn;
    QPushButton* m_syncBtn;
    QPushButton* m_switchBtn;

    QLabel* m_statusLabel;

    QStackedWidget* m_stack;
    QWidget* m_historyWidget;
    QWidget* m_diagramWidget;

    QChart* m_chart;
    QChartView* m_chartView;

    bool m_running = false;
};
#endif // MAINWINDOW_H
