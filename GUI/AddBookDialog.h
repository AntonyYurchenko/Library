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
    explicit AddBookDialog(QList<QString> list, QWidget *parent = 0);

    ~AddBookDialog();

    QList<QString> GetData();

private:
    void Init();

private slots:
    void ValidateOkBtn();

    void on_calendarWidget_clicked(const QDate &date);

    void on_dateEdit_dateChanged(const QDate &date);

private:
    Ui::AddBookDialog *_ui;

    QList<QString> _list;
};

#endif // ADDBOOKDIALOG_H
