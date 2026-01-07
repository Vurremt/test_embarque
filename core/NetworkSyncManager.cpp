#include "NetworkSyncManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>

NetworkSyncManager::NetworkSyncManager(const QString& url, const QString& apiKey, QObject* parent):
    QObject(parent),
    m_url(url),
    m_apiKey(apiKey)
{
    connect(&m_manager, &QNetworkAccessManager::finished, this, &NetworkSyncManager::onReplyFinished);
}

void NetworkSyncManager::syncMeasurements(const std::vector<Measurement>& m)
{
    QJsonArray arr;

    for (const auto& meas : m) {
        QJsonObject obj;
        obj["sensor"] = QString::fromStdString(meas.sensorName);
        obj["value"] = meas.value;
        obj["timestamp"] =
            std::chrono::duration_cast<std::chrono::seconds>(
                meas.timestamp.time_since_epoch()
                ).count();
        arr.append(obj);
    }

    QJsonObject root;
    root["measurements"] = arr;

    QJsonDocument doc(root);

    QNetworkRequest req(m_url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-API-KEY", m_apiKey.toUtf8());

    m_manager.post(req, doc.toJson());
}

void NetworkSyncManager::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError)
        emit syncFinished(false, reply->errorString());
    else
        emit syncFinished(true, "Sync OK");

    reply->deleteLater();
}
