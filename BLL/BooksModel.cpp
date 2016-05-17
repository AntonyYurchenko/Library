#include "BooksModel.h"
#include <QDebug>

BooksModel::BooksModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setHorizontalHeaderItem(0, new QStandardItem(tr("Name")));
    setHorizontalHeaderItem(1, new QStandardItem(tr("Author")));
    setHorizontalHeaderItem(2, new QStandardItem(tr("Date")));
}

BooksModel::~BooksModel()
{
}

void BooksModel::AddBook(const QString studentId, const QList<QString> &data)
{
    FillModel(studentId, data);

    emit ModelChanged();
}

void BooksModel::DeleteBook(const int index)
{
    removeRow(index);

    emit ModelChanged();
}

void BooksModel::DeleteBooksByStudentId(const QString id)
{
    int count = 0;

    for(int j = 0; j < rowCount(); j++)
        if (item(j, 0)->data().toString() == id)
            count++;

    qDebug() << "count: " << count;

    for (int i = 0; i < count; ++i) {
        for(int j = 0; j < rowCount(); j++)
        {
            if (item(j, 0)->data().toString() == id)
            {
                qDebug() << item(j,0)->text();
                removeRow(j);
                break;
            }
        }
    }
}

void BooksModel::FillModel(const QString studentId, const QList<QString> &data)
{
    QList<QStandardItem*> itemsRow;

    QStandardItem *nameItem = new QStandardItem(data.at(0));
    nameItem->setData(studentId);
    nameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    itemsRow << nameItem;

    QStandardItem *authorItem = new QStandardItem(data.at(1));
    authorItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    itemsRow << authorItem;

    QStandardItem *dateItem = new QStandardItem(data.at(2));
    dateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    itemsRow << dateItem;

    appendRow(itemsRow);
}
