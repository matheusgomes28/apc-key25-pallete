#include <RtMidi.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <spdlog/spdlog.h>
#include <cxxopts.hpp>

#include <array>
#include <unordered_map>

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

    enum class ButtonState { Off, Green, GreenBlink, Red, RedBlink, Yellow, YellowBlink };

    // clang-format off
    static constexpr std::array<ButtonState, 8 * 5> INITIAL_STATE{
      // Row 1
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      // Row 2
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      // Row 3
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      // Row 4
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      // Row 5
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
      ButtonState::Yellow,
      ButtonState::Green,
      ButtonState::Red,
    };
    // clang-format on

    void print_devices(std::vector<std::string> const& devices) {
        for (auto const& device_name : devices) {
            fmt::println("device name: {}", device_name);
        }
    }
} // namespace

int main(int argc, const char** argv) {

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

        // Note On: 144, 64, 90
        // message[0] = 144;
        // message[1] = 64;
        // message[2] = 90;
        // midiout->sendMessage( &message );

        std::vector<unsigned char> message{0x90, 0x00, 0x00};
        for (std::size_t i = 0; i < INITIAL_STATE.size(); ++i) {

            ButtonState state = INITIAL_STATE[i];
            message[1]        = i;
            spdlog::info("message for position {} sent", message[1]);

            switch (state) {
            case ButtonState::Off:
                message[2] = 0x00;
                break;
            case ButtonState::Green:
                message[2] = 0x01;
                break;
            case ButtonState::GreenBlink:
                message[2] = 0x02;
                break;
            case ButtonState::Red:
                message[2] = 0x03;
                break;
            case ButtonState::RedBlink:
                message[2] = 0x04;
                break;
            case ButtonState::Yellow:
                message[2] = 0x05;
                break;
            case ButtonState::YellowBlink:
                message[2] = 0x06;
                break;
            }
            midi_out.sendMessage(&message);
        }
    }

    midi_out.closePort();

    return 0;
}
