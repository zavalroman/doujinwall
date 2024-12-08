#ifndef VK_OBJECTS_H
#define VK_OBJECTS_H

#include <QObject>
#include <QJsonObject>

struct Attachment {
    QString type;
    virtual ~Attachment() {}
};

struct ImageCopy {
    QString type;
    QString url;
    int width;
    int height;
};

struct Photo : Attachment {
    int id;
    int album_id;
    int owner_id;
    int user_id;
    QString text;
    int date;
    QList<ImageCopy> sizes;
    int width;
    int height;
};

struct File : Attachment {
    int id;
    int owner_id;
    QString title;
    int size;
    QString ext;
    QString url;
    int date;
    int ftype;
};

struct RawAttachment : Attachment {
    QJsonObject obj;
};

struct WallPost {
    int id;
    int owner_id;
    int from_id;
    int created_by;
    int date;
    QString text;
    int reply_owner_id;
    int reply_post_id;
    int friends_only;
    int commentsCount;
    int likesCount;
    int repostsCount;
    // QList<Attachment> attachments;
    QList<Photo> images;
    QList<File> files;
    QList<RawAttachment> rawAttachments;
};

#endif // VK-OBJECTS_H
