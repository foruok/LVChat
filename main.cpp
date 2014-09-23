#include <QGuiApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include "messengerManager.h"
#include "accessPointModel.h"
#include "pointSizeToPixelSize.h"
#include "../stockMonitor/qDebug2Logcat.h"
#include "audiorecorder.h"
#include <QTranslator>
#include <QIcon>

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("LVChat");
    QGuiApplication app(argc, argv);
    QIcon icon(":/res/appicon.png");
    app.setWindowIcon(icon);
    QTranslator ts;
    ts.load(":/LVChat.qm");
    app.installTranslator(&ts);
    qmlRegisterType<PeerInfo>("an.qt.LVChat", 1, 0, "PeerInfo");
    MessengerManager *mgr = MessengerManager::instance();
    SimpleAudioRecorder *recorder = new SimpleAudioRecorder(QDir::currentPath());
    QQmlApplicationEngine engine;
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("msgManager", mgr);
    ctx->setContextProperty("apmodel", new AccessPointModel);
    ctx->setContextProperty("fontUtil", new PointSizeToPixelSize);
    ctx->setContextProperty("audioRecorder", recorder);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
