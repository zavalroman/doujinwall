#include "vk-api.h"

#include <QJsonDocument>

#include "vk-parser.h"

VkApi::VkApi(QObject *parent)
    : QObject{parent}
{
    rest = new RestClient(this, "https://api.vk.com/method/");
}

void VkApi::wallGet(int offset, int count,
                    std::function<void(const QList<WallPost>&)> callback)
{
    QString url = QString("wall.get?domain=doujinmusic&offset=%1&count=%2&v=5.199&access_token=1589dcb51589dcb51589dcb59b15ddaefa115891589dcb54a1334c5b86bd37ee8d2e92b")
                    .arg(offset).arg(count);
    rest->get(token, url, [callback](const QJsonDocument& reply) {
        callback(VkParser::wallPosts(reply));
    });
}
