// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#include <winapi/utf8.hpp>

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace winapi {
namespace {

std::runtime_error error(const char* function, DWORD code) {
    std::ostringstream oss;
    oss << "Function " << function << " failed with error code " << code;
    return std::runtime_error{oss.str()};
}

bool size_t_to_int(std::size_t src, int32_t& dest) {
    if (src > static_cast<uint32_t>(INT32_MAX))
        return false;
    dest = static_cast<int32_t>(src);
    return true;
}

bool int_to_size_t(int32_t src, std::size_t& dest) {
    if (src < 0 || static_cast<uint32_t>(src) > SIZE_MAX)
        return false;
    dest = static_cast<std::size_t>(src);
    return true;
}

int32_t convert_input_bytes_to_bytes(std::size_t nb) {
    int32_t real_nb = 0;

    if (!size_t_to_int(nb, real_nb)) {
        std::ostringstream oss;
        oss << "Input buffer is too large at " << nb << " bytes";
        throw std::runtime_error{oss.str()};
    }

    return real_nb;
}

int32_t convert_input_bytes_to_chars(std::size_t nb) {
    if (nb % sizeof(WCHAR) != 0) {
        std::ostringstream oss;
        oss << "Buffer size invalid at " << nb << " bytes";
        throw std::runtime_error{oss.str()};
    }

    const std::size_t nch = nb / sizeof(WCHAR);
    int32_t real_nch = 0;

    if (!size_t_to_int(nch, real_nch)) {
        std::ostringstream oss;
        oss << "Input buffer is too large at " << nch << " characters";
        throw std::runtime_error{oss.str()};
    }

    return real_nch;
}

template <typename CharT>
std::vector<CharT> output_buffer(int32_t size) {
    std::size_t real_size = 0;

    if (!int_to_size_t(size, real_size)) {
        std::ostringstream oss;
        oss << "Buffer size invalid at " << size << " bytes";
        throw std::runtime_error{oss.str()};
    }

    std::vector<CharT> buffer;
    buffer.resize(real_size);
    return buffer;
}

template <typename CharT>
void verify_output(const std::vector<CharT>& expected, int32_t _actual_size) {
    std::size_t actual_size = 0;

    if (!int_to_size_t(_actual_size, actual_size) || expected.size() != actual_size) {
        std::ostringstream oss;
        oss << "Expected output length " << expected.size() << ", got " << _actual_size;
        throw std::runtime_error{oss.str()};
    }
}

} // namespace

std::wstring widen(const std::string& src) {
    return widen(src.c_str(), src.size());
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
    static_assert(sizeof(wchar_t) == sizeof(WCHAR), "wchar_t != WCHAR");
    return narrow(src.c_str(), src.size() * sizeof(std::wstring::value_type));
}

std::string narrow(const std::u16string& src) {
    return narrow(src.c_str(), src.size() * sizeof(std::u16string::value_type));
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
