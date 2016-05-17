#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>

namespace Ui {
    class AddStudentDialog;
}

class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = 0);
    ~AddStudentDialog();

    QList<QString> GetData();

private slots:
    void ValidateOkBtn();

private:
    Ui::AddStudentDialog *_ui;
    QList<QString> _list;
};

#endif // ADDSTUDENTDIALOG_H
