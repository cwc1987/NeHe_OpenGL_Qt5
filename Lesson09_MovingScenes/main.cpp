#include <QApplication>
#include <QSurfaceFormat>
#include "moveingsceneswindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    MoveingScenesWindow window;
    window.setFormat(format);
    window.show();
    window.setAnimating(true);
    return app.exec();
}
