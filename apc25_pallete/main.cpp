#include <common/button_state.hpp>

#include <RtMidi.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>

#include <array>
#include <optional>

namespace {

    struct ProgramArgs {
        std::string device_name;
        bool show_devices;
        bool show_help;
    };

    std::optional<ProgramArgs> parse_args(int argc, const char** argv) {
        cxxopts::Options options{"APC25 Pallete", "Utility Program For APC25"};

        // clang-format off
        options.add_options()
          ("d,device-name", "MIDI device name", cxxopts::value<std::string>())
          ("s,show-devices", "Only print the device names")
          ("h,help", "Print usage help");
        // clang-format on

        try {
            auto const result = options.parse(argc, argv);

            if (result.count("help") > 0) {
                spdlog::info(options.help());
                return ProgramArgs{
                 .device_name  = "",
                 .show_devices = false,
                 .show_help    = true,
                };
            }

            if (result.count("device-name") <= 0) {
                spdlog::error("--device-name must be given");
                spdlog::info(options.help());
                return std::nullopt;
            }

            return ProgramArgs{
             .device_name  = result["device-name"].as<std::string>(),
             .show_devices = result.count("show-devices") > 0,
             .show_help    = false,
            };
        } catch (cxxopts::exceptions::exception const& e) {
            spdlog::error("could not parse options: {}", e.what());
            return std::nullopt;
        }
    }

    // clang-format off
    static const std::map<apc25::ButtonState, unsigned char> COLOR_MESSAGES{{
     {apc25::ButtonState::Off,        0x00},
     {apc25::ButtonState::Green,      0x01},
     {apc25::ButtonState::GreenBlink, 0x02},
     {apc25::ButtonState::Red,        0x03},
     {apc25::ButtonState::RedBlink,   0x04},
     {apc25::ButtonState::Yellow,     0x05},
     {apc25::ButtonState::Yellow,     0x06},
    }};

    static constexpr std::array<apc25::ButtonState, 8 * 5> INITIAL_STATE{
      // Row 1
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      // Row 2
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      // Row 3
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      // Row 4
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      // Row 5
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
      apc25::ButtonState::Yellow,
      apc25::ButtonState::Green,
      apc25::ButtonState::Red,
    };
    // clang-format on


    std::optional<std::vector<unsigned char>> makeColorPackets(unsigned char knob_position, apc25::ButtonState button_state) {

        auto const found = COLOR_MESSAGES.find(button_state);
        if (found == end(COLOR_MESSAGES)) {
            return std::nullopt;
        }

        return std::optional<std::vector<unsigned char>>(
            {0x90, knob_position, static_cast<unsigned char>(button_state)});
    }

    void print_devices(std::vector<std::string> const& devices) {
        for (auto const& device_name : devices) {
            fmt::println("device name: {}", device_name);
        }
    }
} // namespace

int main(int argc, const char** argv) {
    static_assert(sizeof(INITIAL_STATE) == 40, "Initial colours must have size of 40!");

    auto const maybe_args = parse_args(argc, argv);
    if (!maybe_args) {
        return -1;
    }
    if (maybe_args->show_help) {
        return 0;
    }
    RtMidiOut midi_out;

    std::vector<std::string> devices;
    std::unordered_map<std::string, int> device_ports;

    // Check outputs.
    auto const nPorts = midi_out.getPortCount();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
    for (unsigned int i = 0; i < nPorts; i++) {
        try {
            std::string const port_name = midi_out.getPortName(i);
            device_ports[port_name]     = i;
            devices.push_back(midi_out.getPortName(i));
        } catch (RtMidiError& error) {
            fmt::println("{}", error.what());
        }
    }

    // MODE: only show devices
    if (maybe_args->show_devices) {
        print_devices(devices);
        return 0;
    }

    // MODE: send the mesages to the device
    if (!maybe_args->device_name.empty()) {
        auto const found = device_ports.find(maybe_args->device_name);
        if (found == end(device_ports)) {
            spdlog::error("device name '{}' does not have a valid port number", maybe_args->device_name);
            return -1;
        }

        midi_out.openPort(found->second);

        for (std::size_t i = 0; i < INITIAL_STATE.size(); ++i) {

            auto maybe_packets = makeColorPackets(static_cast<unsigned char>(i), INITIAL_STATE[i]);
            if (!maybe_packets) {
                spdlog::error("could not get a valid message for");
                return -1;
            }

            midi_out.sendMessage(&(*maybe_packets));
            spdlog::info("message for position {} sent", maybe_packets->at(1));
        }
    }

    midi_out.closePort();
    return 0;
}
