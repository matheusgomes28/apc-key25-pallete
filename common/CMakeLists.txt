# common/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Core)

add_library(apc25_common INTERFACE)

target_include_directories(apc25_common INTERFACE include)

target_link_libraries(apc25_common INTERFACE Qt6::Core)

