#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDate>
#include <QMessageBox>

#include "AddStudentDialog.h"
#include "AddBookDialog.h"
#include "JsonParser.h"

#include <QtPrintSupport/QPrinter>

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
    _ui->printBtn->setEnabled(false);
    _ui->actionPrint->setEnabled(false);
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

    _report = new ReportGenerator();
    _printPreview = new QPrintPreviewDialog(this);
    _printPreview->setWindowTitle(tr("Print"));

    connect(_studentsModel, SIGNAL(ModelChanged()),
            this, SLOT(OnStudentModelChanged()));

    connect(_booksModel, SIGNAL(ModelChanged()),
            this, SLOT(OnBookModelChanged()));

    connect(_ui->menuFile, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));
    connect(_ui->menuEdit, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));
    connect(_ui->menuAbout, SIGNAL(triggered(QAction*)),
            this, SLOT(OnMenuClicked(QAction*)));

    connect(_printPreview, SIGNAL(paintRequested(QPrinter*)),
            this, SLOT(OnPaintRequested(QPrinter*)));
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
        _ui->printBtn->setEnabled(true);
        _ui->actionPrint->setEnabled(true);
    } else {
        _ui->booksTable->hide();
        _ui->noBooksLabel->show();
        _ui->deleteBookBtn->setEnabled(false);
        _ui->changeBookBtn->setEnabled(false);
        _ui->printBtn->setEnabled(false);
        _ui->actionPrint->setEnabled(false);
    }
}

void MainWindow::OnMenuClicked(QAction *action)
{
    if(action == _ui->actionExit)
        QCoreApplication::quit();
    if(action == _ui->actionAdd_student)
        on_addBtn_clicked();
    if(action == _ui->actionPrint)
        on_printBtn_clicked();
    if(action == _ui->actionDeveloper)
    {
        QMessageBox::about(this, tr("Developer"), tr("Develope by Anton Yurchenko\n"
                                                  "for coursework in 2016 BSUIR"));
    }
    if(action == _ui->actionSoftware)
    {
        QMessageBox::about(this, tr("Software"), tr("This software was create to\n"
                                                    "help manage books in\n"
                                                    "university library"));
    }
    if (action == _ui->actionCancel_last_action)
    {
        JsonParser::load("students.txt", _studentsModel);
        JsonParser::load("books.txt", _booksModel);

        _ui->actionCancel_last_action->setEnabled(false);

        CheckBooks();
    }
}

void MainWindow::OnPaintRequested(QPrinter *printer)
{
    QTextDocument *doc = _report->Generate();
    doc->print(printer);
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

void MainWindow::on_printBtn_clicked()
{
    _report->SetReportName(tr("Books student ")  + _studentsModel->item(_ui->studentsTable->currentIndex().row(), 0)->text());

    _report->AddText(tr("Group: ") + _studentsModel->item(_ui->studentsTable->currentIndex().row(), 2)->text() + "\n" +
                     tr("Department: ") + _studentsModel->item(_ui->studentsTable->currentIndex().row(), 1)->text(), QFont(), Qt::AlignLeft);

    _report->AddTable(_ui->booksTable->model(),
                      _ui->booksTable->horizontalHeader(),
                      _ui->booksTable->horizontalHeader());

    _printPreview->exec();
}
