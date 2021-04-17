winapi-utf8
===========

[![CI](https://github.com/egor-tensin/winapi-utf8/actions/workflows/ci.yml/badge.svg)](https://github.com/egor-tensin/winapi-utf8/actions/workflows/ci.yml)

UTF-8 <-> UTF-16 conversion functions, mainly to be used with WinAPI.

Development
-----------

Build using CMake.
Depends on Boost.Test.
The project is Windows-only, so building with either MSVC or MinGW-w64 is
required.

There's a Makefile with useful shortcuts to build the project in the .build/
directory (defaults to building with MinGW-w64):

    make deps
    make build
    make test

License
-------

Distributed under the MIT License.
See [LICENSE.txt] for details.

[LICENSE.txt]: LICENSE.txt
