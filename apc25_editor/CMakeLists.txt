set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_executable(apc25_editor main.cpp)

qt_add_qml_module(apc25_editor
    URI apc25editor
    QML_FILES
        main.qml
    NO_CACHEGEN
)

target_link_libraries(apc25_editor PRIVATE Qt6::Quick)

