#include "doujin-database.h"

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDir>

#define DB_CONNECTION_NAME "DOUJIN_DATABASE"

DoujinDatabase::DoujinDatabase(QObject *parent)
    : QObject{parent}
{}

void DoujinDatabase::init()
{
    QSqlDatabase database = QSqlDatabase::database(DB_CONNECTION_NAME);
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION_NAME);
        if (!database.isValid()) {
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
            return;
        }
    }

    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDir.mkpath(".")) {
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));
        return;
    }

    // Ensure that we have a writable location on all devices.
    const QString fileName = writeDir.absolutePath() + "/doujin-database.sqlite3";
    qDebug() << "Database path:" << fileName;

    // When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
        return;
    }

    QSqlQuery query(database);
    query.exec("CREATE TABLE IF NOT EXISTS post ("
               "id             INTEGER NOT NULL DEFAULT 0, "
               "owner_id       INTEGER NOT NULL DEFAULT 0, "
               "from_id        INTEGER NOT NULL DEFAULT 0, "
               "created_by     INTEGER NOT NULL DEFAULT 0, "
               "reply_owner_id INTEGER NOT NULL DEFAULT 0, "
               "reply_post_id  INTEGER NOT NULL DEFAULT 0, "
               "comments_count INTEGER NOT NULL DEFAULT 0, "
               "likes_count    INTEGER NOT NULL DEFAULT 0, "
               "reposts_count  INTEGER NOT NULL DEFAULT 0, "
               "date           INTEGER NOT NULL DEFAULT 0, "
               "text TEXT NOT NULL, "
               "PRIMARY KEY (id))");

    query.exec("CREATE TABLE IF NOT EXISTS image ("
               "id             INTEGER NOT NULL DEFAULT 0, "
               "post_id        INTEGER NOT NULL DEFAULT 0, "
               "album_id       INTEGER NOT NULL DEFAULT 0, "
               "owner_id       INTEGER NOT NULL DEFAULT 0, "
               "user_id        INTEGER NOT NULL DEFAULT 0, "
               "width          INTEGER NOT NULL DEFAULT 0, "
               "height         INTEGER NOT NULL DEFAULT 0, "
               "date           INTEGER NOT NULL DEFAULT 0, "
               "text TEXT NOT NULL, "
               "url  TEXT NOT NULL, "
               "PRIMARY KEY (id))");

    query.exec("CREATE TABLE IF NOT EXISTS file ("
               "id             INTEGER NOT NULL DEFAULT 0, "
               "post_id        INTEGER NOT NULL DEFAULT 0, "
               "owner_id       INTEGER NOT NULL DEFAULT 0, "
               "size           INTEGER NOT NULL DEFAULT 0, "
               "date           INTEGER NOT NULL DEFAULT 0, "
               "title TEXT NOT NULL, "
               "ext   TEXT NOT NULL, "
               "url   TEXT NOT NULL, "
               "PRIMARY KEY (id))");
}

bool DoujinDatabase::insertPosts(const QList<WallPost>& posts)
{
    QSqlDatabase database = QSqlDatabase::database(DB_CONNECTION_NAME);

    for (auto& p : posts) {
        QString postId = QString::number(p.id);
        QSqlQuery query(database);

        QString queryString = "INSERT INTO post (id, owner_id, from_id, created_by, "
                              "reply_owner_id, reply_post_id, comments_count, "
                              "likes_count, reposts_count, date, text) "
                              "VALUES (" + postId                 + ", "
                              + QString::number(p.owner_id)       + ", "
                              + QString::number(p.from_id)        + ", "
                              + QString::number(p.created_by)     + ", "
                              + QString::number(p.reply_owner_id) + ", "
                              + QString::number(p.reply_post_id)  + ", "
                              + QString::number(p.commentsCount)  + ", "
                              + QString::number(p.likesCount)     + ", "
                              + QString::number(p.repostsCount)   + ", "
                              + QString::number(p.date)           + ", '"
                              + p.text                            + "')";
        if (!query.exec(queryString)) {
            qDebug() << "ERROR! Post query:" << queryString;
            qDebug() << "SQL Error:" << query.lastError();
            return false;
        }

        for (auto& i : p.images) {
            auto it = std::max_element(i.sizes.begin(), i.sizes.end(), [](const ImageCopy& a, const ImageCopy& b) {
                return a.width < b.width; // Сравниваем по ширине
            });

            QString imageQueryString = "INSERT INTO image (id, post_id, album_id, owner_id, "
                                  "user_id, width, height, date, text, url) "
                                  "VALUES (" + QString::number(i.id) + ", "
                                  + postId                           + ", "
                                  + QString::number(i.album_id)      + ", "
                                  + QString::number(i.owner_id)      + ", "
                                  + QString::number(i.user_id)       + ", "
                                  + QString::number(it->width)         + ", "
                                  + QString::number(it->height)        + ", "
                                  + QString::number(i.date)          + ", '"
                                  + i.text                           + "', '"
                                  + it->url                         + "')";
            if (!query.exec(imageQueryString)) {
                qDebug() << "ERROR! Image query:" << imageQueryString;
                qDebug() << "SQL Error:" << query.lastError();
                return false;
            }
        }

        for (auto& f : p.files) {
            QString fileQueryString = "INSERT INTO file (id, post_id, owner_id, size, "
                                      "date, title, ext, url) "
                                       "VALUES (" + QString::number(f.id) + ", "
                                       + postId                           + ", "
                                       + QString::number(f.owner_id)      + ", "
                                       + QString::number(f.size)          + ", "
                                       + QString::number(f.date)          + ", '"
                                       + f.title                          + "', '"
                                       + f.ext                            + "', '"
                                       + f.url                            + "')";
            if (!query.exec(fileQueryString)) {
                qDebug() << "ERROR! File query:" << fileQueryString;
                qDebug() << "SQL Error:" << query.lastError();
                return false;
            }
        }
    }
    return true;
}
