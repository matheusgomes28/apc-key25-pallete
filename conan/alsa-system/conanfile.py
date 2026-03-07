from conan import ConanFile


class AlsaSystemConan(ConanFile):
    name = "libalsa"
    version = "system"

    def package_info(self):
        self.cpp_info.system_libs = ["asound", "jack"]
