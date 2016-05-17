#ifndef STUDENTSMODEL_H
#define STUDENTSMODEL_H

#include <QStandardItemModel>

class StudentsModel : public QStandardItemModel
{
    Q_OBJECT

public:
    StudentsModel(QObject *parent = 0);
    ~StudentsModel();

    void AddStudent(const QList<QString> &list);
    void DeleteStudent(const int index);

signals:
    void ModelChanged();

private:
    void FillModel(const QList<QString> &list);

    void Save();
    void Load();
};

#endif // STUDENTSMODEL_H
