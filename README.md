winapi-utf8
===========

[![CI](https://github.com/egor-tensin/winapi-utf8/actions/workflows/ci.yml/badge.svg)](https://github.com/egor-tensin/winapi-utf8/actions/workflows/ci.yml)

UTF-8 <-> UTF-16 conversion functions, mainly to be used with WinAPI.

Usage
-----

Include it in your CMake project and link to the `winapi_utf8` target.
Use the `narrow()` and `widen()` functions to convert to UTF-8 and UTF-16
respectively:

    #include <winapi/utf8.hpp>

    winapi::narrow(u"Привет"); // Returns the UTF-8 representation
    winapi::widen(u8"Привет"); // Returns the UTF-16 representation

See the various convenience overloads of these function in the
[online documentation].

[online documentation]: https://egor-tensin.github.io/winapi-utf8/utf8_8hpp.html

Development
-----------

Build using CMake.
Depends on Boost.Test.
The project is Windows-only, so building with either MSVC or MinGW-w64 is
required.

There's a Makefile with useful shortcuts to build the project in the build/
directory (defaults to building with MinGW-w64):

    make deps
    make build
    make test

Documentation
-------------

Build & display the documentation using

    make docs

View the online documentation at https://egor-tensin.github.io/winapi-utf8/.

License
-------

Distributed under the MIT License.
See [LICENSE.txt] for details.

[LICENSE.txt]: LICENSE.txt
