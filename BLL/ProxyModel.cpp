#include "ProxyModel.h"

ProxyModel::ProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

ProxyModel::~ProxyModel()
{
}

void ProxyModel::SetFilter(const QString &filter)
{
    _filter = filter;
    invalidate();
}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex itemIndex = sourceModel()->index(source_row, 0, source_parent);

    bool result = AcceptModelIndex(itemIndex, Qt::UserRole + 1);

    return result;
}

bool ProxyModel::AcceptModelIndex(const QModelIndex &index, const int &filterType) const
{
    if (_filter == sourceModel()->data(index, filterType).toString())
        return true;
    else
        return false;
}
