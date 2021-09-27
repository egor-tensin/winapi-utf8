// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

/**
 * \file
 * \brief UTF-8 <-> UTF-16 conversion functions
 */

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace winapi {

/** Convert UTF-8 string to UTF-16. */
std::wstring widen(const std::string&);

/**
 * Convert UTF-8 string to UTF-16.
 * \param src Memory address of a UTF-8 string.
 * \param nb  String size (in bytes).
 */
std::wstring widen(const void* src, std::size_t nb);

/**
 * Convert UTF-8 string to UTF-16.
 * \param src Buffer holding a UTF-8 string.
 */
template <typename T, typename Alloc = std::allocator<T>>
std::wstring widen(const std::vector<T, Alloc>& src) {
    return widen(src.data(), src.size() * sizeof(T));
}

/** Convert UTF-16 string to UTF-8. */
std::string narrow(const std::wstring&);

/**
 * Convert UTF-16 string to UTF-8.
 * \param src Memory address of a UTF-16 string.
 * \param nb  String size (in bytes).
 */
std::string narrow(const void* src, std::size_t nb);

/**
 * Convert UTF-16 string to UTF-8.
 * \param src Buffer holding a UTF-16 string.
 */
template <typename T, typename Alloc = std::allocator<T>>
std::string narrow(const std::vector<T, Alloc>& src) {
    return narrow(src.data(), src.size() * sizeof(T));
}

} // namespace winapi
