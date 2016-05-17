#include "AddStudentDialog.h"
#include "ui_AddStudentDialog.h"

#include <QPushButton>

AddStudentDialog::AddStudentDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AddStudentDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(_ui->nameEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
    connect(_ui->departmentEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
    connect(_ui->groupEdit, SIGNAL(textChanged(QString)),
        this, SLOT(ValidateOkBtn()));
}

AddStudentDialog::~AddStudentDialog()
{
    delete _ui;
}

QList<QString> AddStudentDialog::GetData()
{
    QList<QString> list;
    list << _ui->nameEdit->text()
         << _ui->departmentEdit->text()
         << _ui->groupEdit->text();

    return list;
}

void AddStudentDialog::ValidateOkBtn()
{
    if (_ui->nameEdit->text().isEmpty() ||
        _ui->departmentEdit->text().isEmpty() ||
        _ui->groupEdit->text().isEmpty())
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}


