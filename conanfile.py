import os

from conans import ConanFile, tools


class PowerIteratorConan(ConanFile):
    name = "power_iterator"
    version = "1.0"
    license = "MIT"
    author = "John Szwast <And your email here>"
    url = "https://github.com/Pafnoutios/memoized-member"
    description = "Iterators over the power set or combinations of a collection."
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "include/*"
    no_copy_source = True
    # No settings/options are necessary, this is header only

    default_user = 'jszwast'
    default_channel = 'stable'

    build_requires = ["gtest/1.10.0"]
    generators = ["CMakeToolchain", "cmake_find_package_multi"]

    def requirements(self):
        self.requires("memoized_member/1.2@{}/{}".format(self.user, self.channel))

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
