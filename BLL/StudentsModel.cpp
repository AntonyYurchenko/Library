#include "StudentsModel.h"

#include <QUuid>

StudentsModel::StudentsModel(QObject *parent) :
    QStandardItemModel(parent)
{
    setHorizontalHeaderItem(0, new QStandardItem(tr("Name")));
    setHorizontalHeaderItem(1, new QStandardItem(tr("Department")));
    setHorizontalHeaderItem(2, new QStandardItem(tr("Group")));
}

StudentsModel::~StudentsModel()
{
}

void StudentsModel::AddStudent(const QList<QString> &list)
{
    FillModel(list);

    emit ModelChanged();
}

void StudentsModel::DeleteStudent(const int index)
{
    removeRow(index);

    emit ModelChanged();
}

void StudentsModel::FillModel(const QList<QString> &list)
{
    QList<QStandardItem*> itemsRow;

    QStandardItem *nameItem = new QStandardItem(list.at(0));
    nameItem->setData(QUuid().createUuid().toString());
    nameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    itemsRow << nameItem;

    QStandardItem *departmentItem = new QStandardItem(list.at(1));
    departmentItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    itemsRow << departmentItem;

    QStandardItem *groupItem = new QStandardItem(list.at(2));
    groupItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    itemsRow << groupItem;

    appendRow(itemsRow);
}
