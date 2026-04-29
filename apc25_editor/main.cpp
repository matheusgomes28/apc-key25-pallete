#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringLiteral>

int main(int argc, char** argv) {
    using namespace Qt::StringLiterals;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/qt/qml/apc25editor/main.qml"_s);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );
    engine.load(url);

    return app.exec();
}
