// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#include <winapi/utf8.hpp>

#include <SafeInt.hpp>

#include <windows.h>

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace winapi {
namespace {

std::runtime_error error(const char* function, DWORD code) {
    std::ostringstream oss;
    oss << function << " failed with error code " << code;
    return std::runtime_error{oss.str()};
}

int convert_input_bytes_to_bytes(std::size_t nb) {
    int real_nb = 0;

    if (!SafeCast(nb, real_nb)) {
        std::ostringstream oss;
        oss << "input buffer is too large at " << nb << " bytes";
        throw std::runtime_error{oss.str()};
    }

    return real_nb;
}

int convert_input_bytes_to_chars(std::size_t nb) {
    if (nb % sizeof(WCHAR) != 0) {
        std::ostringstream oss;
        oss << "invalid buffer size: " << nb;
        throw std::runtime_error{oss.str()};
    }

    const std::size_t nch = nb / sizeof(WCHAR);

    int real_nch = 0;

    if (!SafeCast(nch, real_nch)) {
        std::ostringstream oss;
        oss << "input buffer is too large at " << nch << " characters";
        throw std::runtime_error{oss.str()};
    }

    return real_nch;
}

template <typename CharT>
std::vector<CharT> output_buffer(int size) {
    std::size_t real_size = 0;

    if (!SafeCast(size, real_size)) {
        std::ostringstream oss;
        oss << "invalid buffer size " << size;
        throw std::runtime_error{oss.str()};
    }

    std::vector<CharT> buffer;
    buffer.resize(real_size);
    return buffer;
}

template <typename CharT>
void verify_output(const std::vector<CharT>& expected, int actual_size) {
    if (!SafeEquals(expected.size(), actual_size)) {
        std::ostringstream oss;
        oss << "expected output length " << expected.size() << ", got " << actual_size;
        throw std::runtime_error{oss.str()};
    }
}

} // namespace

std::wstring widen(const std::string& src) {
    return widen(src.c_str(), src.size());
}

std::wstring widen(const std::vector<unsigned char>& src) {
    return widen(src.data(), src.size());
}

std::wstring widen(const void* src, std::size_t in_nb) {
    const DWORD flags = MB_ERR_INVALID_CHARS;

    const auto in_data = reinterpret_cast<const char*>(src);
    const auto real_in_nb = convert_input_bytes_to_bytes(in_nb);

    auto out_nch = ::MultiByteToWideChar(CP_UTF8, flags, in_data, real_in_nb, NULL, 0);

    if (out_nch == 0) {
        throw error("MultiByteToWideChar", GetLastError());
    }

    static_assert(sizeof(wchar_t) == sizeof(WCHAR), "wchar_t != WCHAR");
    auto out = output_buffer<wchar_t>(out_nch);

    out_nch = ::MultiByteToWideChar(CP_UTF8, flags, in_data, real_in_nb, out.data(), out_nch);

    if (out_nch == 0) {
        throw error("MultiByteToWideChar", GetLastError());
    }

    verify_output(out, out_nch);
    return {out.data(), out.size()};
}

std::string narrow(const std::wstring& src) {
    static_assert(sizeof(std::wstring::value_type) == sizeof(WCHAR), "wchar_t != WCHAR");
    return narrow(src.c_str(), src.size() * sizeof(std::wstring::value_type));
}

std::string narrow(const std::vector<unsigned char>& src) {
    return narrow(src.data(), src.size());
}

std::string narrow(const void* src, std::size_t in_nb) {
    const DWORD flags = WC_ERR_INVALID_CHARS;

    const auto in_data = reinterpret_cast<const wchar_t*>(src);
    const auto in_nch = convert_input_bytes_to_chars(in_nb);

    auto out_nb = ::WideCharToMultiByte(CP_UTF8, flags, in_data, in_nch, NULL, 0, NULL, NULL);

    if (out_nb == 0) {
        throw error("WideCharToMultiByte", GetLastError());
    }

    auto out = output_buffer<char>(out_nb);

    out_nb = ::WideCharToMultiByte(CP_UTF8, flags, in_data, in_nch, out.data(), out_nb, NULL, NULL);

    if (out_nb == 0) {
        throw error("WideCharToMultiByte", GetLastError());
    }

    verify_output(out, out_nb);
    return {out.data(), out.size()};
}

} // namespace winapi
