#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtSerialPort/QtSerialPort>
#include <QQmlEngine>
#include <QQmlContext>
#include <serialcomm.h>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QScopedPointer<serialComm> serialHMI(new serialComm);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    engine.rootContext()->setContextProperty("serialHMI",serialHMI.data());
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
