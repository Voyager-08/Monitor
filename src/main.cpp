#include <QApplication>
#include <QFile>

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Monitor");
    QApplication::setOrganizationName("Monitor");

    QFile styleFile(":/qss/style.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));

    MainWindow window;
    window.show();

    return app.exec();
}
