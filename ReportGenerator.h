#pragma once

#include <QTextDocument>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTextTable>

class ReportGenerator : public QObject
{
    Q_OBJECT

    typedef QPair<QAbstractItemModel *, QString> Table;
    typedef QPair<QHeaderView *, QHeaderView *> TableHeaders;
    typedef QPair<QFont, Qt::Alignment> TextFormat;
    typedef QPair<QString, TextFormat> Text;

public:
    explicit ReportGenerator(QObject *parent = 0);
    virtual ~ReportGenerator();

    void SetReportName(const QString &reportName);

    void AddTable(QAbstractItemModel *tableModel,
                  QHeaderView *horizontalHeader,
                  QHeaderView *verticalHeader,
                  const QString &tableTitle = QString());

    void AddText(const QString &text,
                 const QFont &font = QFont(),
                 const Qt::Alignment &alignmentFlag = Qt::AlignLeft|Qt::AlignCenter);

    QTextDocument *Generate();

private:
    void InsertReportName(QTextCursor &cursor);
    void InsertTable(QTextCursor &cursor, const Table &table, const TableHeaders &tableHeaders, bool enableTitleCounter, int tableNumber);
    void HideColumn(QHeaderView *hHeader, QTextTable *table);
    void InsertText(QTextCursor &cursor, const Text &text);

private:
    QString _reportName;
    QMap<quint32, Table> _models;
    QMap<quint32, TableHeaders> _tableHeaders;
    QMap<quint32, Text> _text;
};
