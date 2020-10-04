// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#include <winapi/utf8.hpp>

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

}

std::wstring widen(const std::string& src) {
    return widen(src.c_str(), src.size());
}

std::wstring widen(const std::vector<unsigned char>& src) {
    return widen(src.data(), src.size());
}

std::wstring widen(const void* src, std::size_t in_nb) {
    const DWORD flags = MB_ERR_INVALID_CHARS
                      | MB_PRECOMPOSED;

    const char* in_data = reinterpret_cast<const char*>(src);

    auto out_nch = ::MultiByteToWideChar(CP_UTF8, flags, in_data, in_nb, NULL, 0);

    if (out_nch == 0) {
        throw error("MultiByteToWideChar", GetLastError());
    }

    static_assert(sizeof(wchar_t) == sizeof(WCHAR), "wchar_t != WCHAR");
    std::vector<wchar_t> out;
    out.resize(out_nch);

    out_nch = ::MultiByteToWideChar(CP_UTF8, flags, in_data, in_nb, out.data(), out.size());

    if (out_nch == 0) {
        throw error("MultiByteToWideChar", GetLastError());
    }

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
    if (in_nb % sizeof(WCHAR) != 0) {
        std::ostringstream err_msg;
        err_msg << "narrow: invalid buffer size: " << in_nb;
        throw std::runtime_error{err_msg.str()};
    }

    const std::size_t in_nch = in_nb / sizeof(WCHAR);

    const DWORD flags = WC_ERR_INVALID_CHARS
                      | WC_NO_BEST_FIT_CHARS;

    const wchar_t* in_data = reinterpret_cast<const wchar_t*>(src);

    auto out_nb = ::WideCharToMultiByte(CP_UTF8, flags, in_data, in_nch, NULL, 0, NULL, NULL);

    if (out_nb == 0) {
        throw error("WideCharToMultiByte", GetLastError());
    }

    std::vector<char> out;
    out.resize(out_nb);

    out_nb = ::WideCharToMultiByte(CP_UTF8, flags, in_data, in_nch, out.data(), out.size(), NULL, NULL);

    if (out_nb == 0) {
        throw error("WideCharToMultiByte", GetLastError());
    }

    return {out.data(), out.size()};
}

}
