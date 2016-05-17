#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "StudentsModel.h"
#include "BooksModel.h"
#include "ProxyModel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void PrepareTable(QTableView *table);
    void CheckBooks();

private slots:
    void on_addBtn_clicked();
    void on_deleteBtn_clicked();

    void on_studentsTable_clicked(const QModelIndex &index);
    void on_booksTable_clicked(const QModelIndex &index);

    void on_addBookBtn_clicked();
    void on_changeBookBtn_clicked();
    void on_deleteBookBtn_clicked();

    void OnStudentModelChanged();
    void OnBookModelChanged();

private:
    Ui::MainWindow *_ui;

    QString _studentId;

    ProxyModel *_proxyModel;

    StudentsModel *_studentsModel;
    BooksModel *_booksModel;
};

#endif // MAINWINDOW_H
