#include "DatabaseManager.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>
#include <QDebug>

DatabaseManager::DatabaseManager(const QString& path): m_path(path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_path);
}

bool DatabaseManager::open(){
    if (!m_db.open()) {
        qDebug() << "Failed to open DB:" << m_db.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::init(){
    return createTables();
}

bool DatabaseManager::insertMeasurement(const Measurement& m){
    QSqlQuery q;
    q.prepare("INSERT INTO measurements(sensor, value, timestamp) VALUES(?, ?, ?)");
    q.addBindValue(QString::fromStdString(m.sensorName));
    q.addBindValue(m.value);
    q.addBindValue( std::chrono::duration_cast<std::chrono::seconds>( m.timestamp.time_since_epoch() ).count() );

    if (!q.exec()) {
        qDebug() << "Insert failed:" << q.lastError().text();
        return false;
    }

    return true;
}

std::vector<Measurement> DatabaseManager::getLastMeasurements(int count){
    std::vector<Measurement> result;

    QSqlQuery q;
    q.prepare("SELECT sensor, value, timestamp FROM measurements ORDER BY id DESC LIMIT ?");
    q.addBindValue(count);

    if (!q.exec()) {
        qDebug() << "Select failed:" << q.lastError().text();
        return result;
    }

    while (q.next()) {
        Measurement m;
        m.sensorName = q.value(0).toString().toStdString();
        m.value = q.value(1).toDouble();
        auto ts = q.value(2).toLongLong();
        m.timestamp = std::chrono::system_clock::time_point{
            std::chrono::seconds(ts)
        };
        result.push_back(m);
    }

    return result;
}

bool DatabaseManager::createTables(){
    QSqlQuery q;
    QString sql = "CREATE TABLE IF NOT EXISTS measurements (id INTEGER PRIMARY KEY AUTOINCREMENT, sensor TEXT, value REAL, timestamp INTEGER);";

    if (!q.exec(sql)) {
        qDebug() << "Failed to create table:" << q.lastError().text();
        return false;
    }
    return true;
}
