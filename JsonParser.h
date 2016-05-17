#ifndef MODELSTORAGE_H
#define MODELSTORAGE_H

#include <QStandardItemModel>
#include <QStandardItem>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JsonParser
{
public:
    static bool load(QString fileName, QStandardItemModel *model);
    static bool save(QString fileName, QStandardItemModel *model);
};

#endif // MODELSTORAGE_H
