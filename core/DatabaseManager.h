#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QtSql/QSqlDatabase>

#include <Measurement.h>

class DatabaseManager {
public:
    explicit DatabaseManager(const QString& path);

    bool open();
    bool init();
    bool insertMeasurement(const Measurement& m);
    std::vector<Measurement> getLastMeasurements(int count);

private:
    bool createTables();

private:
    QSqlDatabase m_db;
    QString m_path;
};

#endif // DATABASEMANAGER_H
