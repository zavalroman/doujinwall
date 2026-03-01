#include "vk-api.h"

#include <QJsonDocument>

#include "vk-parser.h"

VkApi::VkApi(QObject *parent)
    : QObject{parent}
{
    rest = new RestClient(this, "https://api.vk.com/method/");
}

void VkApi::wallGet(int offset, int count,
                    std::function<void(const QList<WallPost>&, bool success)> callback)
{
    QString url = QString("wall.get?domain=doujinmusic&offset=%1&count=%2&v=5.199")
                    .arg(offset).arg(count);
    qDebug() << "URL" << url;
    rest->get(token, url, [callback](const QJsonDocument& reply, bool success) {
        callback(VkParser::wallPosts(reply), success);
    });
}
