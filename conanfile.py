from conan import ConanFile


class WavyTuneConan(ConanFile):
    settings = ("os", "compiler", "build_type", "arch")
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("cxxopts/3.2.0")
        self.requires("fmt/11.2.0")
        self.requires("gtest/1.17.0")
        self.requires("ms-gsl/4.1.0")
        self.requires("rtmidi/6.0.0")
        self.requires("spdlog/1.15.3")
        self.requires("libalsa/system", override=True)

    def configure(self):
        self.options["sqlitecpp/*"].with_sqlcipher = True
        self.options["podofo/*"].with_jpeg = "libjpeg"
