#include "AddBookDialog.h"
#include "ui_AddBookDialog.h"

#include <QPushButton>
#include <QDebug>

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AddBookDialog)
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

void AddBookDialog::ValidateOkBtn()
{
    if (_ui->nameEdit->text().isEmpty() ||
        _ui->authorEdit->text().isEmpty() ||
        _ui->dateEdit->text().isEmpty())
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
