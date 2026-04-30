#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringLiteral>
#include <QQmlContext>

#include "cell_model.hpp"

int main(int argc, char** argv) {
    using namespace Qt::StringLiterals;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    CellModel model{5, 8};
    engine.rootContext()->setContextProperty("cellModel", &model);

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
