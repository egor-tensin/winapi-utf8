// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace winapi {

std::wstring widen(const std::string&);
std::wstring widen(const std::vector<unsigned char>&);
std::wstring widen(const void*, std::size_t nb);

std::string narrow(const std::wstring&);
std::string narrow(const wchar_t*, std::size_t nch);
std::string narrow(const std::vector<unsigned char>&);
std::string narrow(const void*, std::size_t nb);

} // namespace winapi
