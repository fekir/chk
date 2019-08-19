#!/usr/bin/env python

from conans import ConanFile, CMake

class chkConan(ConanFile):
    name = "chk"
    description = "C++ library for compile-time tests and assertions"
    url = "https://github.com/fekir/chk"
    homepage = url
    license = "ISC"
    exports = ("LICENSE", "readme.adoc")
    version = "1.0"
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = ("include/*", "CMakeLists.txt", "tests/*")
    no_copy_source = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_id(self):
        self.info.header_only()
