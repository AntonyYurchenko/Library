#include "AddBookDialog.h"
#include "ui_AddBookDialog.h"

#include <QPushButton>
#include <QDebug>

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
    _ui->dateEdit->setText(list.at(2));
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
         << _ui->dateEdit->text();

    return list;
}

void AddBookDialog::Init()
{
    _ui->setupUi(this);
    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(_ui->nameEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
    connect(_ui->authorEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
    connect(_ui->dateEdit, SIGNAL(textChanged(QString)),
            this, SLOT(ValidateOkBtn()));
}

void AddBookDialog::ValidateOkBtn()
{
    if (_ui->nameEdit->text().isEmpty() ||
        _ui->authorEdit->text().isEmpty() ||
        _ui->dateEdit->text().isEmpty())
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
