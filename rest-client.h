#ifndef REST_CLIENT_H
#define REST_CLIENT_H

#include <QObject>
#include <QRestAccessManager>
#include <QNetworkAccessManager>

class RestClient : public QObject
{
    Q_OBJECT
public:
    explicit RestClient(QObject *parent = nullptr, QString apiUrl = "Develop");

    void get(const QString& token,
             const QString& url,
             std::function<void (QJsonDocument, bool)> callback);

    void post(const QString &token,
              const QString& url,
              const QByteArray& data,
              std::function<void (QJsonDocument, bool)> callback);

private:
    const QString TAG = "RestClient";

    QString apiUrl;

    QRestAccessManager* restManager = nullptr;
    QNetworkAccessManager* networkManager = nullptr;

    QNetworkRequest createRequest(const QString& token, const QString& url);

    void finish(QRestReply &reply, std::function<void (QJsonDocument, bool)> callback);
};

#endif // REST_CLIENT_H
