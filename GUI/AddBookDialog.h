#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
    class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = 0);
    ~AddBookDialog();

    QList<QString> GetData();

private slots:
    void ValidateOkBtn();

private:
    Ui::AddBookDialog *_ui;

    QList<QString> _list;
};

#endif // ADDBOOKDIALOG_H
