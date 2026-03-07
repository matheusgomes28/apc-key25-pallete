find_package(fmt REQUIRED)
find_package(RtMidi REQUIRED)
find_package(cxxopts REQUIRED)
find_package(spdlog REQUIRED)

add_executable(apc25_pallete)

target_sources(apc25_pallete PRIVATE main.cpp)

target_link_libraries(apc25_pallete
  PRIVATE
    RtMidi::rtmidi
    fmt::fmt
    cxxopts::cxxopts
    spdlog::spdlog)

