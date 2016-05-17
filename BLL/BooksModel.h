#ifndef BOOKSMODEL_H
#define BOOKSMODEL_H

#include <QStandardItemModel>

class BooksModel : public QStandardItemModel
{
    Q_OBJECT

public:
    BooksModel(QObject *parent);
    ~BooksModel();

    void AddBook(const QString studentId, const QList<QString> &data);
    void DeleteBook(const int index);
    void DeleteBooksByStudentId(const QString id);

signals:
    void ModelChanged();

private:
    void FillModel(const QString studentId, const QList<QString> &data);
};

#endif // BOOKSMODEL_H
