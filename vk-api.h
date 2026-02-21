#ifndef VK_API_H
#define VK_API_H

#include <QObject>

#include "rest-client.h"
#include "vk-objects.h"

class VkApi : public QObject
{
    Q_OBJECT
public:
    explicit VkApi(QObject *parent = nullptr);

    void setToken(const QString& value) {
        this->token = value;
    }

    void wallGet(int offset, int count,
                 std::function<void (const QList<WallPost> &, bool)> callback);

signals:

private:
    QString token;
    RestClient* rest;
};

#endif // VK_API_H
