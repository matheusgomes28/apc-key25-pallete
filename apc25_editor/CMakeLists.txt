# apc25_editor/CMakeLists.txt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

qt_add_executable(apc25_editor
  main.cpp
  cell_model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../common/include/common/button_state.hpp)

set_target_properties(apc25_editor PROPERTIES AUTOMOC ON)

qt_add_qml_module(apc25_editor
    URI apc25editor
    QML_FILES
        main.qml
    NO_CACHEGEN
)

target_link_libraries(apc25_editor PRIVATE Qt6::Quick apc25_common)

