// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace winapi {

std::wstring widen(const std::string&);
std::wstring widen(const void*, std::size_t nb);

template <typename T, typename Alloc = std::allocator<T>>
std::wstring widen(const std::vector<T, Alloc>& src) {
    return widen(src.data(), src.size() * sizeof(T));
}

std::string narrow(const std::wstring&);
std::string narrow(const void*, std::size_t nb);

template <typename T, typename Alloc = std::allocator<T>>
std::string narrow(const std::vector<T, Alloc>& src) {
    return narrow(src.data(), src.size() * sizeof(T));
}

} // namespace winapi
