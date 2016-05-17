#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator* translator = new QTranslator();
    if (translator->load("ru",":/resources/translations"))
    {
        app.installTranslator(translator);
    }

    app.setStyleSheet("");

    MainWindow w;
    w.show();

    return app.exec();
}
