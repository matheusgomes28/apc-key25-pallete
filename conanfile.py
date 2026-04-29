from conan import ConanFile


class WavyTuneConan(ConanFile):
    settings = ("os", "compiler", "build_type", "arch")
    generators = "CMakeDeps", "CMakeToolchain", "QbsDeps"

    def requirements(self):
        self.requires("cxxopts/3.2.0")
        self.requires("fmt/11.2.0")
        self.requires("gtest/1.17.0")
        self.requires("libalsa/system", override=True)
        self.requires("ms-gsl/4.1.0")
        self.requires("qt/6.8.3")
        self.requires("rtmidi/6.0.0")
        self.requires("spdlog/1.15.3")

    def configure(self):
        self.options["podofo/*"].with_jpeg = "libjpeg"
        self.options["qt/*"].qtshadertools = True
        self.options["qt/*"].qtdeclarative = True
        self.options["qt/*"].qtdeClarativetools = True
        self.options["qt/*"].shared = True
        self.options["sqlitecpp/*"].with_sqlcipher = True
