#include <QApplication>
#include <QStyle>
#include <QIcon>

#include "gui/MainWindow.h"
#include "gui/Theme.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");
    QApplication::setApplicationName("Monitor");
    QApplication::setOrganizationName("Monitor");
    QApplication::setWindowIcon(QIcon(":/icons/windowIcon.png"));

    applyTheme(currentThemeName());

    MainWindow window;
    window.show();

    return app.exec();
}
