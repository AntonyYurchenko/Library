#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>

class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ProxyModel(QObject *parent = 0);
   ~ProxyModel();

    void SetFilter(const QString &filter);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    bool AcceptModelIndex(const QModelIndex &index, const int &filterType) const;

private:
    QString _filter;
};

#endif // PROXYMODEL_H
