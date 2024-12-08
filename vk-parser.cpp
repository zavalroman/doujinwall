#include "vk-parser.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

VkParser::VkParser(QObject *parent)
    : QObject{parent}
{}

QList<WallPost> VkParser::wallPosts(const QJsonDocument &doc)
{
    const QJsonArray array = doc.object()["response"].toObject()["items"].toArray();
    QList<WallPost> posts;
    for (auto p : std::as_const(array)) {
        WallPost post;

        QJsonObject obj = p.toObject();
        post.id = obj["id"].toInt();
        post.owner_id = obj["owner_id"].toInt();
        post.from_id = obj["from_id"].toInt();
        post.created_by = obj["created_by"].toInt();
        post.date = obj["date"].toInt();
        post.text = obj["text"].toString();
        post.reply_owner_id = obj["reply_owner_id"].toInt();
        post.reply_post_id = obj["reply_post_id"].toInt();
        post.friends_only = obj["friends_only"].toInt();

        post.commentsCount = obj["comments"].toObject()["count"].toInt();
        post.repostsCount = obj["reposts"].toObject()["count"].toInt();
        post.likesCount = obj["likes"].toObject()["count"].toInt();

        QJsonArray attachmentsArray = obj["attachments"].toArray();
        for (auto it : std::as_const(attachmentsArray)) {
            QJsonObject attachmentObj = it.toObject();
            QString type = attachmentObj["type"].toString();
            QJsonObject typedObj = attachmentObj[type].toObject();
            if (type == "photo") {
                post.images << parsePhoto(typedObj);
            } else if (type == "doc") {
                post.files << parseFile(typedObj);
            } else {
                RawAttachment raw;
                raw.type = type;
                raw.obj = typedObj;
                post.rawAttachments << raw;
            }
            //post.attachments << parseAttachment(it.toObject()); не понимаю как кастить Attachment в дочерние типы
        }
        posts << post;
    }
    return posts;
}

Attachment VkParser::parseAttachment(const QJsonObject &obj)
{
    QString type = obj["type"].toString();
    QJsonObject typedObj = obj[type].toObject();
    if (type == "photo") {
        Photo p;
        p.type = type;

        p.id       = typedObj["id"].toInt();
        p.album_id = typedObj["album_id"].toInt();
        p.owner_id = typedObj["owner_id"].toInt();
        p.user_id  = typedObj["user_id"].toInt();
        p.text     = typedObj["text"].toString();
        p.date     = typedObj["date"].toInt();
        p.width    = typedObj["width"].toInt();
        p.height   = typedObj["height"].toInt();

        QJsonArray sizesArray = obj["sizes"].toArray();
        for (auto it : std::as_const(sizesArray)) {
            QJsonObject icObj = it.toObject();
            ImageCopy ic;
            ic.type   = icObj["type"].toString();
            ic.url    = icObj["url"].toString();
            ic.width  = icObj["width"].toInt();
            ic.height = icObj["height"].toInt();

            p.sizes << ic;
        }
        return p;
    } else if (type == "doc") {
        File f;
        f.type = type;

        f.id       = typedObj["id"].toInt();
        f.owner_id = typedObj["owner_id"].toInt();
        f.title    = typedObj["title"].toString();
        f.size     = typedObj["size"].toInt();
        f.ext      = typedObj["ext"].toString();
        f.url      = typedObj["url"].toString();
        f.date     = typedObj["date"].toInt();
        f.ftype    = typedObj["type"].toInt();

        return f;
    } else {
        RawAttachment a;
        a.type = type;
        a.obj = typedObj;
        return a;
    }
}

Photo VkParser::parsePhoto(const QJsonObject &obj)
{
    Photo p;
    p.type = "photo";

    p.id       = obj["id"].toInt();
    p.album_id = obj["album_id"].toInt();
    p.owner_id = obj["owner_id"].toInt();
    p.user_id  = obj["user_id"].toInt();
    p.text     = obj["text"].toString();
    p.date     = obj["date"].toInt();
    p.width    = obj["width"].toInt();
    p.height   = obj["height"].toInt();

    QJsonArray sizesArray = obj["sizes"].toArray();
    for (auto it : std::as_const(sizesArray)) {
        QJsonObject icObj = it.toObject();
        ImageCopy ic;
        ic.type   = icObj["type"].toString();
        ic.url    = icObj["url"].toString();
        ic.width  = icObj["width"].toInt();
        ic.height = icObj["height"].toInt();

        p.sizes << ic;
    }
    return p;
}

File VkParser::parseFile(const QJsonObject &obj)
{
    File f;
    f.type = "doc";

    f.id       = obj["id"].toInt();
    f.owner_id = obj["owner_id"].toInt();
    f.title    = obj["title"].toString();
    f.size     = obj["size"].toInt();
    f.ext      = obj["ext"].toString();
    f.url      = obj["url"].toString();
    f.date     = obj["date"].toInt();
    f.ftype    = obj["type"].toInt();

    return f;
}
