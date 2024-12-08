#ifndef VK - PARSER_H
#define VK -PARSER_H

#include <QObject>

#include "vk-objects.h"

class VkParser : public QObject
{
    Q_OBJECT
public:
    explicit VkParser(QObject *parent = nullptr);

    static QList<WallPost> wallPosts(const QJsonDocument& doc);

signals:

private:
    static Attachment parseAttachment(const QJsonObject&);
    static Photo parsePhoto(const QJsonObject&);
    static File parseFile(const QJsonObject&);
};

#endif // VK-PARSER_H
