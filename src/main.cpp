#include <QGuiApplication>
#include <QQuickView>
#include "VtkSurfaceItem.h"

int main(int argc, char** argv) {
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setRedBufferSize(1);
    format.setGreenBufferSize(1);
    QSurfaceFormat::setDefaultFormat(format);

    QGuiApplication app(argc, argv);
    qmlRegisterType<VtkSurfaceItem>("com.zeroth.vtk", 1, 0, "VtkSurfaceItem");
    // Your app
    QQuickView view;
    view.setSource(QUrl("qrc:/main.qml"));
    view.setMinimumSize(QSize(640, 480));
    view.show();
    return app.exec();
}
