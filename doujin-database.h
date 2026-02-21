#ifndef DOUJIN_DATABASE_H
#define DOUJIN_DATABASE_H

#include <QObject>

#include "vk-objects.h"

class DoujinDatabase : public QObject
{
    Q_OBJECT
public:
    explicit DoujinDatabase(QObject *parent = nullptr);

    static void init();
    static bool insertPosts(const QList<WallPost>& posts);

    static void createFrontAlbumsList();

signals:
};

#endif // AGENT-DATABASE_H
