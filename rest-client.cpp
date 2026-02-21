#include "rest-client.h"

#include <QRestReply>
#include <QNetworkReply>
#include <QJsonDocument>

RestClient::RestClient(QObject *parent, QString apiUrl)
    : QObject{parent}, apiUrl{std::move(apiUrl)}
{
    networkManager = new QNetworkAccessManager(this);
    restManager = new QRestAccessManager(networkManager, networkManager);
}

void RestClient::get(const QString &token,
                     const QString &url,
                     std::function<void (QJsonDocument, bool)> callback)
{
    //qDebug() << "GET REQUEST PARAMS" << url << token;
    QNetworkRequest request = createRequest(token, url);

    restManager->get(request, this, [this, callback](QRestReply &reply) {
        //qDebug() << "GET REPLY" << reply.readBody();
        finish(reply, callback);
    });
}

void RestClient::post(const QString& token,
                      const QString &url,
                      const QByteArray &data,
                      std::function<void(QJsonDocument doc, bool success)> callback)
{
    QNetworkRequest request = createRequest(token, url);

    restManager->post(request, data, this, [this, callback](QRestReply &reply) {
        qDebug() << "POST REPLY" << reply;
        finish(reply, callback);
    });
}

QNetworkRequest RestClient::createRequest(const QString &token, const QString &url)
{
    QNetworkRequest request(apiUrl + url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (!token.isEmpty()) {
        QString bearerToken = "Bearer " + token;
        request.setRawHeader(QByteArray("Authorization"), QByteArray(bearerToken.toUtf8()));
    }

    return request;
}

void RestClient::finish(QRestReply &reply, std::function<void (QJsonDocument doc, bool success)> callback)
{
    if (!reply.isSuccess()) {
        qDebug() << "HTTP request not successful: " << reply.errorString();
    }
    callback(QJsonDocument::fromJson(reply.readBody()), reply.isSuccess());
}
