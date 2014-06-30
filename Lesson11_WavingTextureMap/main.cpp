#include <QApplication>
#include <QSurfaceFormat>
#include "texturemappingwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TextureMappingWindow window;
    window.setFormat(format);
    window.show();
    window.setAnimating(true);
    return app.exec();
}
