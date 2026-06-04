// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

/**
 * \file
 * \brief UTF-8 <-> UTF-16 conversion functions
 */

#pragma once

#include <concepts>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace winapi {

/* sizeof(wchar_t) == 4 on Linux, we don't care about that. */
static_assert(sizeof(wchar_t) == 2, "This is Windows, right?");

/** Convert UTF-8 string to UTF-16. */
std::wstring widen(std::string_view);
std::wstring widen(std::u8string_view);

/**
 * Convert UTF-8 string to UTF-16.
 * \param src Memory address of a UTF-8 string.
 * \param nb  String size (in bytes).
 */
std::wstring widen(const void* src, std::size_t nb);

template <typename T>
concept CanBeWidened =
    std::same_as<T, char> || std::same_as<T, char8_t> || std::same_as<T, unsigned char>;

/**
 * Convert UTF-8 string to UTF-16.
 * \param src Buffer holding a UTF-8 string.
 */
template <CanBeWidened T, typename Alloc = std::allocator<T>>
std::wstring widen(const std::vector<T, Alloc>& src) {
    return widen(src.data(), src.size() * sizeof(T));
}

/** Convert UTF-16 string to UTF-8. */
std::string narrow(std::wstring_view);

/** Convert UTF-16 string to UTF-8. */
std::string narrow(std::u16string_view);

/**
 * Convert UTF-16 string to UTF-8.
 * \param src Memory address of a UTF-16 string.
 * \param nb  String size (in bytes).
 */
std::string narrow(const void* src, std::size_t nb);

template <typename T>
concept CanBeNarrowed =
    std::same_as<T, wchar_t> || std::same_as<T, char16_t> || std::same_as<T, unsigned char>;

/**
 * Convert UTF-16 string to UTF-8.
 * \param src Buffer holding a UTF-16 string.
 */
template <CanBeNarrowed T, typename Alloc = std::allocator<T>>
std::string narrow(const std::vector<T, Alloc>& src) {
    return narrow(src.data(), src.size() * sizeof(T));
}

} // namespace winapi
