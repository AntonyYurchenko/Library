#include "ReportGenerator.h"

#include <QTextCursor>
#include <QTextTableCell>

ReportGenerator::ReportGenerator(QObject *parent) :
    QObject(parent),
    _reportName("")
{
}

ReportGenerator::~ReportGenerator()
{

}

void ReportGenerator::SetReportName(const QString &reportName)
{
    _reportName = reportName;
}

void ReportGenerator::AddText(const QString &text, const QFont &font, const Qt::Alignment &alignmentFlag)
{
    _text.insert(1, Text(text, TextFormat(font, alignmentFlag)));
}

void ReportGenerator::AddTable(QAbstractItemModel *tableModel,
                                      QHeaderView *horizontalHeader,
                                      QHeaderView *verticalHeader,
                                      const QString &tableTitle)
{
    _models.insert(0, Table(tableModel, tableTitle));
    _tableHeaders.insert(0, TableHeaders(horizontalHeader, verticalHeader));
}

void ReportGenerator::InsertReportName(QTextCursor &cursor)
{
    if(!_reportName.isEmpty())
    {
        QTextBlockFormat textBlockFormat;
        textBlockFormat.setAlignment(Qt::AlignCenter);

        QTextCharFormat textBoldFormat;
        QFont font("Times", 16, QFont::Bold);
        textBoldFormat.setFont(font);
        cursor.setBlockFormat(textBlockFormat);
        cursor.insertText(_reportName + "\n", textBoldFormat);
    }
}

void ReportGenerator::InsertText(QTextCursor &cursor, const ReportGenerator::Text &text)
{
    QString textString = text.first;
    TextFormat textFormat = text.second;

    QTextBlockFormat textBlockFormat;
    textBlockFormat.setAlignment(textFormat.second);

    QTextCharFormat textUserFormat;
    textUserFormat.setFont(textFormat.first);
    cursor.setBlockFormat(textBlockFormat);
    cursor.insertText(textString + "\n", textUserFormat);
}

QTextDocument *ReportGenerator::Generate()
{
    QTextDocument *document = new QTextDocument();
    QTextCursor cursor = QTextCursor(document);

    cursor.movePosition(QTextCursor::Start);

    InsertReportName(cursor);
    InsertText(cursor, _text.value(1));
    InsertTable(cursor, _models.value(0), _tableHeaders.value(0), false, 0);

    return document;
}

void ReportGenerator::InsertTable(QTextCursor &cursor,
                                         const ReportGenerator::Table &table,
                                         const TableHeaders &tableHeaders,
                                         bool enableTitleCounter,
                                         int tableNumber)
{
    QAbstractItemModel *model = table.first;
    const QString tableTitle = table.second;

    QHeaderView* hHeader = tableHeaders.first;

    if(!tableTitle.isEmpty())
    {
        QTextBlockFormat tableTitleTextBlockFormat;
        tableTitleTextBlockFormat.setAlignment(Qt::AlignLeft);

        QTextCharFormat textBoldFormat;
        QFont font("Times New Roman", 14, QFont::Normal);
        textBoldFormat.setFont(font);
        cursor.setBlockFormat(tableTitleTextBlockFormat);
        if (enableTitleCounter)
            cursor.insertText(tr("Table ") + QString::number(tableNumber) +". " + tableTitle, textBoldFormat);
        else
            cursor.insertText(tableTitle, textBoldFormat);
    }

    const int rowCount = qBound(0, model->rowCount(), model->rowCount());
    const int columnCount = model->columnCount();

    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setPageBreakPolicy(QTextTableFormat::PageBreak_Auto);
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderBrush(QBrush(Qt::black));

    QTextFrameFormat frameFormat;
    frameFormat.setBackground(QColor("#e1e1e1"));

    cursor.insertFrame(frameFormat);

    QTextTable *textTable = cursor.insertTable(rowCount+1, columnCount, tableFormat);

    QTextBlockFormat cellHeaderFormat;
    cellHeaderFormat.setAlignment(Qt::AlignCenter);

    QTextTableCell cellheader;
    QTextCharFormat headerTextCharFormat;

    for (int column = 0; column < columnCount; ++column)
    {
        cellheader = textTable->cellAt(0, column);

        cursor = cellheader.firstCursorPosition();

        const QString text = model->headerData(column, Qt::Horizontal,Qt::DisplayRole).toString();

        QFont headerFont("Times New Roman", 12, QFont::Bold);

        cellHeaderFormat.setBackground(QColor("#e1e1e1"));

        headerTextCharFormat.setFont(headerFont);
        cursor.setBlockFormat(cellHeaderFormat);
        cursor.insertText(text, headerTextCharFormat);
    }

    QTextBlockFormat paraFormat;
    paraFormat.setBottomMargin(0);
    paraFormat.setTopMargin(0);

    QTextTableCell cellstart;
    QColor backgroundColor;
    QModelIndex index;
    QTextCharFormat existFormat;
    QTextCharFormat inlineStyle;

    for (int row = 0; row < rowCount; ++row)
    {
        for (int column = 0; column < columnCount; ++column)
        {
            cellstart = textTable->cellAt(row + 1, column);
            index = model->index(row, column);

            if (index.isValid ())
            {
                const QString text = model->data(index,Qt::DisplayRole).toString();

                QFont textFont("Times New Roman", 12, QFont::Normal);
                //                QFont textFont = model->data(index,Qt::FontRole).value<QFont>();
                QColor TextColor = model->data(index,Qt::TextColorRole).value<QColor>();

                backgroundColor = model->data(index,Qt::BackgroundRole).value<QColor>();

                paraFormat.setAlignment((Qt::Alignment)model->data(index,Qt::TextAlignmentRole).toInt());

                if(!TextColor.isValid())
                    TextColor = model->data(index,Qt::ForegroundRole).value<QColor>();

                if(!backgroundColor.isValid())
                    backgroundColor = QColor("#ffffff");

                existFormat = cellstart.format();
                existFormat.setBackground(backgroundColor);
                cellstart.setFormat(existFormat);

                inlineStyle.setForeground(QBrush(TextColor));
                inlineStyle.setFont(textFont);

                cursor = cellstart.firstCursorPosition();
                cursor.setBlockFormat(paraFormat);
                cursor.insertText(text, inlineStyle);
            }
        }
    }

    HideColumn(hHeader, textTable);
}

void ReportGenerator::HideColumn(QHeaderView * hHeader, QTextTable *table)
{
    int logicalIndex;
    QList<int> hiddenColums;

    for (int column = 0; column < hHeader->count(); ++column)
    {
        logicalIndex = hHeader->logicalIndex(column);

        if (hHeader->isSectionHidden(logicalIndex))
        {
            hiddenColums << column;
        }
    }

    if (!hiddenColums.isEmpty())
    {
        foreach (int column, hiddenColums)
        {
            table->removeColumns(column, 1);
        }
    }

}
