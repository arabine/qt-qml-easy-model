#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "device.h"
#include "sensor_model.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    SensorModel sensor_model(&app);
    QQmlApplicationEngine engine;
    qmlRegisterUncreatableType<Device>( "App", 1, 0, "Device", "Device is managed from C++" );

    QQmlContext * context = engine.rootContext();
    context->setContextProperty("sensor_model", &sensor_model);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
