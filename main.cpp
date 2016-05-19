#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator* translator = new QTranslator();
    if (translator->load("ru.qm", ":/translations"))
    {
        app.installTranslator(translator);
    }

    MainWindow w;
    w.show();

    return app.exec();
}
