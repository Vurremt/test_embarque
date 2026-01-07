#ifndef NETWORKSYNCMANAGER_H
#define NETWORKSYNCMANAGER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <vector>

#include "Measurement.h"

class NetworkSyncManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkSyncManager(const QString& url, const QString& apiKey, QObject* parent = nullptr);

    void syncMeasurements(const std::vector<Measurement>& m);

signals:
    void syncFinished(bool success, QString message);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_manager;
    QString m_url;
    QString m_apiKey;
};

#endif // NETWORKSYNCMANAGER_H
