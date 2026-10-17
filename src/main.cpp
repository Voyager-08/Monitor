#include <QApplication>
#include <QStyle>
#include <QFile>

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");
    QApplication::setApplicationName("Monitor");
    QApplication::setOrganizationName("Monitor");
    QApplication::setWindowIcon(QIcon(":/icons/windowIcon.png"));

    QFile styleFile(":/qss/style.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));

    MainWindow window;
    window.show();

    return app.exec();
}