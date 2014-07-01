#include <QApplication>
#include "fontwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    FontWindow window;
    window.setFormat(format);
    window.show();
    window.setAnimating(true);
    return app.exec();
}
