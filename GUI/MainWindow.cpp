#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include "AddStudentDialog.h"
#include "AddBookDialog.h"
#include "JsonParser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _studentId("null")
{
    _ui->setupUi(this);

    _ui->bookGroupBox->hide();
    _ui->noBooksLabel->hide();

    _ui->deleteBtn->setEnabled(false);
    _ui->deleteBookBtn->setEnabled(false);
    _ui->changeBookBtn->setEnabled(false);

    _studentsModel = new StudentsModel(this);
    _ui->studentsTable->setModel(_studentsModel);
    PrepareTable(_ui->studentsTable);

    _booksModel = new BooksModel(this);

    JsonParser::load("students.txt", _studentsModel);
    JsonParser::load("books.txt", _booksModel);

    _proxyModel = new ProxyModel(this);

    _proxyModel->setSourceModel(_booksModel);
    _ui->booksTable->setModel(_proxyModel);

    PrepareTable(_ui->booksTable);

    connect(_studentsModel, SIGNAL(ModelChanged()),
            this, SLOT(OnStudentModelChanged()));

    connect(_booksModel, SIGNAL(ModelChanged()),
            this, SLOT(OnBookModelChanged()));
}

MainWindow::~MainWindow()
{
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);

    delete _ui;
}

void MainWindow::PrepareTable(QTableView *table)
{
    table->verticalHeader()->setVisible(table == _ui->booksTable);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::OnStudentModelChanged()
{
    if (_studentsModel->rowCount() == 0)
    {
        _ui->bookGroupBox->hide();
        _ui->chooseLabel->show();
    }
    else
    {
        _ui->deleteBtn->setEnabled(false);
    }
}

void MainWindow::OnBookModelChanged()
{
    if (_proxyModel->rowCount() != 0) {
        _ui->booksTable->show();
        _ui->noBooksLabel->hide();
    } else {
        _ui->booksTable->hide();
        _ui->noBooksLabel->show();
        _ui->deleteBookBtn->setEnabled(false);
        _ui->changeBookBtn->setEnabled(false);
    }
}

void MainWindow::on_addBtn_clicked()
{
    AddStudentDialog *addStudentDialog = new AddStudentDialog(this);

   if (addStudentDialog->exec() == QDialog::Accepted)
   {
        _studentsModel->AddStudent(addStudentDialog->GetData());
   }
}

void MainWindow::on_deleteBtn_clicked()
{
    _studentsModel->DeleteStudent(_ui->studentsTable->currentIndex().row());
    _booksModel->DeleteBooksByStudentId(_studentId);
}

void MainWindow::on_studentsTable_clicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        _ui->bookGroupBox->show();
        _ui->chooseLabel->hide();

        _ui->deleteBtn->setEnabled(true);
        _studentId = _studentsModel->item(index.row())->data().toString();
        _proxyModel->SetFilter(_studentId);

        OnBookModelChanged();
    }
}

void MainWindow::on_booksTable_clicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        _ui->deleteBookBtn->setEnabled(true);
        _ui->changeBookBtn->setEnabled(true);
    }
}

void MainWindow::on_addBookBtn_clicked()
{
    AddBookDialog *addBookDialog = new AddBookDialog(this);

   if (addBookDialog->exec() == QDialog::Accepted)
   {
        _booksModel->AddBook(_studentId, addBookDialog->GetData());
   }
}

void MainWindow::on_changeBookBtn_clicked()
{
    QList<QString> list;
    list << _booksModel->item(_ui->booksTable->currentIndex().row(), 0)->text()
         << _booksModel->item(_ui->booksTable->currentIndex().row(), 1)->text()
         << _booksModel->item(_ui->booksTable->currentIndex().row(), 2)->text();

    AddBookDialog *addBookDialog = new AddBookDialog(list, this);

   if (addBookDialog->exec() == QDialog::Accepted)
   {
        _booksModel->item(_ui->booksTable->currentIndex().row(), 0)->setText(addBookDialog->GetData().at(0));
        _booksModel->item(_ui->booksTable->currentIndex().row(), 1)->setText(addBookDialog->GetData().at(1));
        _booksModel->item(_ui->booksTable->currentIndex().row(), 2)->setText(addBookDialog->GetData().at(2));
   }
}

void MainWindow::on_deleteBookBtn_clicked()
{
    _booksModel->DeleteBook(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row());
}
