#include "AddBookDialog.h"
#include "ui_AddBookDialog.h"

#include <QPushButton>

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AddBookDialog)
{
    Init();
}

AddBookDialog::AddBookDialog(QList<QString> list, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AddBookDialog)
{
    Init();

    _ui->nameEdit->setText(list.at(0));
    _ui->authorEdit->setText(list.at(1));
    _ui->dateEdit->setDisplayFormat("dd.MM.yyyy");
    _ui->dateEdit->setDate(QDate::fromString(list.at(2), "dd.MM.yyyy"));

    if (list.at(3) == tr("Returned"))
        _ui->returnBox->setCheckState(Qt::Checked);
    else
        _ui->returnBox->setCheckState(Qt::Unchecked);
}

AddBookDialog::~AddBookDialog()
{
    delete _ui;
}

QList<QString> AddBookDialog::GetData()
{
    QList<QString> list;
    list << _ui->nameEdit->text()
         << _ui->authorEdit->text()
         << _ui->dateEdit->date().toString("dd.MM.yyyy");

    if (_ui->returnBox->isChecked())
        list << tr("Returned");
    else
        list << tr("In use");

    return list;
}

void AddBookDialog::Init()
{
    _ui->setupUi(this);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    _ui->dateEdit->setDate(QDate::currentDate());
    _ui->dateEdit->setDisplayFormat("dd.MM.yyyy");
//    _ui->dateEdit->setMinimumDate(QDate::currentDate());

    connect(_ui->nameEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
    connect(_ui->authorEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
}

void AddBookDialog::ValidateOkBtn()
{
    if (_ui->nameEdit->text().isEmpty() ||
        _ui->authorEdit->text().isEmpty())
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void AddBookDialog::on_calendarWidget_clicked(const QDate &date)
{
    _ui->dateEdit->setDate(date);
}

void AddBookDialog::on_dateEdit_dateChanged(const QDate &date)
{
    _ui->calendarWidget->setSelectedDate(date);
}
