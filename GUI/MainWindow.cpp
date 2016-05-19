#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

#include "AddStudentDialog.h"
#include "AddBookDialog.h"
#include "JsonParser.h"
#include <QDate>

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
    _ui->actionCancel_last_action->setEnabled(false);

    _studentsModel = new StudentsModel(this);
    _ui->studentsTable->setModel(_studentsModel);
    PrepareTable(_ui->studentsTable);

    _booksModel = new BooksModel(this);

    JsonParser::load("students.txt", _studentsModel);
    JsonParser::load("books.txt", _booksModel);

    CheckBooks();

    _proxyModel = new ProxyModel(this);

    _proxyModel->setSourceModel(_booksModel);
    _ui->booksTable->setModel(_proxyModel);

    PrepareTable(_ui->booksTable);

    connect(_studentsModel, SIGNAL(ModelChanged()),
            this, SLOT(OnStudentModelChanged()));

    connect(_booksModel, SIGNAL(ModelChanged()),
            this, SLOT(OnBookModelChanged()));

    connect(_ui->menuFile, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));
    connect(_ui->menuEdit, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));
    connect(_ui->menuHelp, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));
    connect(_ui->menuAbout, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));
}

MainWindow::~MainWindow()
{
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);

    delete _ui;
}

void MainWindow::PrepareTable(QTableView *table)
{
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::CheckBooks()
{
    QList<QString> ids;

    for (int i = 0; i < _booksModel->rowCount(); i++)
        if (QDate::fromString(_booksModel->item(i,2)->text(), "dd.MM.yyyy") <
            QDate::currentDate() && _booksModel->item(i,3)->text() != tr("Returned"))
        {
            _booksModel->item(i,0)->setBackground(QBrush(QColor(255,100,100)));
            _booksModel->item(i,1)->setBackground(QBrush(QColor(255,100,100)));
            _booksModel->item(i,2)->setBackground(QBrush(QColor(255,100,100)));
            _booksModel->item(i,3)->setBackground(QBrush(QColor(255,100,100)));

            ids << _booksModel->item(i,0)->data().toString();
        }
    else if (_booksModel->item(i,3)->text() == tr("Returned"))
        {
            _booksModel->item(i,0)->setBackground(QBrush(QColor(71,214,97)));
            _booksModel->item(i,1)->setBackground(QBrush(QColor(71,214,97)));
            _booksModel->item(i,2)->setBackground(QBrush(QColor(71,214,97)));
            _booksModel->item(i,3)->setBackground(QBrush(QColor(71,214,97)));
        }
    else if (QDate::fromString(_booksModel->item(i,2)->text(), "dd.MM.yyyy") >
             QDate::currentDate() && _booksModel->item(i,3)->text() != tr("Returned"))
        {
            _booksModel->item(i,0)->setBackground(QBrush(Qt::white));
            _booksModel->item(i,1)->setBackground(QBrush(Qt::white));
            _booksModel->item(i,2)->setBackground(QBrush(Qt::white));
            _booksModel->item(i,3)->setBackground(QBrush(Qt::white));
        }
    for (int i = 0; i < _studentsModel->rowCount(); i++)
        if (ids.contains(_studentsModel->item(i,0)->data().toString()))
        {
            _studentsModel->item(i,0)->setBackground(QBrush(QColor(255,100,100)));
            _studentsModel->item(i,1)->setBackground(QBrush(QColor(255,100,100)));
            _studentsModel->item(i,2)->setBackground(QBrush(QColor(255,100,100)));
        }
        else
        {
            _studentsModel->item(i,0)->setBackground(QBrush(QColor(Qt::white)));
            _studentsModel->item(i,1)->setBackground(QBrush(QColor(Qt::white)));
            _studentsModel->item(i,2)->setBackground(QBrush(QColor(Qt::white)));
        }
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

void MainWindow::OnMenuClicked(QAction *action)
{
    if(action == _ui->actionExit)
        QCoreApplication::quit();
    if(action == _ui->actionAdd_student)
        on_addBtn_clicked();
    if(action == _ui->actionGuide)
    {
        QMessageBox::about(this, tr("Guide"), tr("If you need details how to work\n"
                                                 "with program, please contact the developer"));
    }

    if(action == _ui->actionAuthor)
    {
        QMessageBox::about(this, tr("Author"), tr("Develope by Anton Yurchenko\n"
                                                  "for coursework in 2016 BSUIR"));
    }
    if(action == _ui->actionSoftware)
    {
        QMessageBox::about(this, tr("Software"), tr("This software was create to\n"
                                                    "help manage books in\n"
                                                    "university library"));
    }
    if(action == _ui->actionVersion)
    {
        QMessageBox::aboutQt(this, tr("Version"));
    }
    if (action == _ui->actionCancel_last_action)
    {
        JsonParser::load("students.txt", _studentsModel);
        JsonParser::load("books.txt", _booksModel);
        _ui->actionCancel_last_action->setEnabled(false);
    }
}

void MainWindow::on_addBtn_clicked()
{
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);

    AddStudentDialog *addStudentDialog = new AddStudentDialog(this);

   if (addStudentDialog->exec() == QDialog::Accepted)
   {
        _studentsModel->AddStudent(addStudentDialog->GetData());
        _ui->actionCancel_last_action->setEnabled(true);
   }
}

void MainWindow::on_deleteBtn_clicked()
{
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);
    _ui->actionCancel_last_action->setEnabled(true);

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
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);

    AddBookDialog *addBookDialog = new AddBookDialog(this);

   if (addBookDialog->exec() == QDialog::Accepted)
   {
        _booksModel->AddBook(_studentId, addBookDialog->GetData());
        _ui->actionCancel_last_action->setEnabled(true);
   }

   CheckBooks();
}

void MainWindow::on_changeBookBtn_clicked()
{
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);

    QList<QString> list;
    list << _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 0)->text()
         << _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 1)->text()
         << _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 2)->text()
         << _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 3)->text();

    AddBookDialog *addBookDialog = new AddBookDialog(list, this);

   if (addBookDialog->exec() == QDialog::Accepted)
   {
        _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 0)->setText(addBookDialog->GetData().at(0));
        _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 1)->setText(addBookDialog->GetData().at(1));
        _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 2)->setText(addBookDialog->GetData().at(2));
        _booksModel->item(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row(), 3)->setText(addBookDialog->GetData().at(3));
        _ui->actionCancel_last_action->setEnabled(true);
   }

   CheckBooks();
}

void MainWindow::on_deleteBookBtn_clicked()
{
    JsonParser::save("students.txt", _studentsModel);
    JsonParser::save("books.txt", _booksModel);
    _ui->actionCancel_last_action->setEnabled(true);

    _booksModel->DeleteBook(_proxyModel->mapToSource(_ui->booksTable->currentIndex()).row());
}
