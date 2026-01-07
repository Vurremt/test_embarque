#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>

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

private slots:
    void onMeasurements(const std::vector<Measurement>& m);
    void onStartStop();
    void onSyncNow();
    void onSyncStatus(bool ok, const QString& message);

private:
    AcquisitionController* m_controller;

    QTableWidget* m_liveTable;
    QTableWidget* m_historyTable;

    QPushButton* m_startStopBtn;
    QPushButton* m_syncBtn;

    QLabel* m_statusLabel;

    bool m_running = false;
};
#endif // MAINWINDOW_H
