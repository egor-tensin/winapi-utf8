// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#include <boost/test/unit_test.hpp>

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

namespace {

std::string convert(const char* src, std::size_t nch) {
    return std::string{src, nch};
}

std::string convert(const char* src) {
    return std::string{src};
}

std::wstring convert(const wchar_t* src, std::size_t nch) {
    return std::wstring{src, nch};
}

std::wstring convert(const wchar_t* src) {
    return std::wstring{src};
}

}

BOOST_AUTO_TEST_SUITE(string_tests)

BOOST_AUTO_TEST_CASE(length) {
    {
        std::string empty;
        BOOST_TEST(empty.size() == 0);
        BOOST_TEST(empty.length() == 0);
    }
    {
        std::wstring empty;
        BOOST_TEST(empty.size() == 0);
        BOOST_TEST(empty.length() == 0);
    }
    {
        std::string s = "asdf";
        BOOST_TEST(s.size() == 4);
        BOOST_TEST(s.length() == 4);
    }
    {
        std::wstring s = L"asdf";
        BOOST_TEST(s.size() == 4);
        BOOST_TEST(s.length() == 4);
    }

    {
        std::string s = "as\0df";
        BOOST_TEST(s.size() == 2);
        BOOST_TEST(s.length() == 2);
    }
    {
        std::string s = "\0asdf";
        BOOST_TEST(s.size() == 0);
        BOOST_TEST(s.length() == 0);
    }
}

BOOST_AUTO_TEST_CASE(null_terminated_narrow) {
    const std::vector<char> src{'a', 'b', 'c', '\0', '1', '2', '3'};
    BOOST_TEST(src.size() == 7);

    {
        const auto converted = convert(src.data(), 7);
        BOOST_TEST(converted.size() == 7);
        BOOST_TEST(std::memcmp(converted.c_str(), src.data(), 7) == 0);
        BOOST_TEST(converted.c_str()[3] == '\0');
        BOOST_TEST(converted.c_str()[7] == '\0');
    }
    {
        const auto converted = convert(src.data(), 4);
        BOOST_TEST(converted.size() == 4);
        BOOST_TEST(std::memcmp(converted.c_str(), src.data(), 4) == 0);
        BOOST_TEST(converted.c_str()[3] == '\0');
        BOOST_TEST(converted.c_str()[4] == '\0');
    }
    {
        const auto converted = convert(src.data(), 3);
        BOOST_TEST(converted.size() == 3);
        BOOST_TEST(std::memcmp(converted.c_str(), src.data(), 3) == 0);
        BOOST_TEST(converted.c_str()[3] == '\0');
    }
    {
        const auto converted = convert(src.data());
        BOOST_TEST(converted.size() == 3);
        BOOST_TEST(converted == "abc");
    }
    {
        const auto converted = convert(src.data() + 2);
        BOOST_TEST(converted.size() == 1);
        BOOST_TEST(converted == "c");
    }
}

BOOST_AUTO_TEST_CASE(null_terminated_wide) {
    const std::vector<wchar_t> src{L'\0', L'a', L'b', L'c', L'\0', L'1', L'2', L'3'};
    BOOST_TEST(src.size() == 8);

    {
        const auto converted = convert(src.data(), 8);
        BOOST_TEST(converted.size() == 8);
        BOOST_TEST(std::memcmp(converted.c_str(), src.data(), 8 * sizeof(wchar_t)) == 0);
        BOOST_TEST(converted.c_str()[0] == L'\0');
        BOOST_TEST(converted.c_str()[4] == L'\0');
        BOOST_TEST(converted.c_str()[8] == L'\0');
    }
    {
        const auto converted = convert(src.data(), 5);
        BOOST_TEST(converted.size() == 5);
        BOOST_TEST(std::memcmp(converted.c_str(), src.data(), 5 * sizeof(wchar_t)) == 0);
        BOOST_TEST(converted.c_str()[0] == L'\0');
        BOOST_TEST(converted.c_str()[4] == L'\0');
        BOOST_TEST(converted.c_str()[5] == L'\0');
    }
    {
        const auto converted = convert(src.data(), 4);
        BOOST_TEST(converted.size() == 4);
        BOOST_TEST(std::memcmp(converted.c_str(), src.data(), 4 * sizeof(wchar_t)) == 0);
        BOOST_TEST(converted.c_str()[0] == L'\0');
        BOOST_TEST(converted.c_str()[4] == L'\0');
    }
    {
        const auto converted = convert(src.data());
        BOOST_TEST(converted.size() == 0);
    }
    {
        const auto converted = convert(src.data() + 1);
        BOOST_TEST(converted.size() == 3);
        BOOST_TEST(converted[0] == L'a');
        BOOST_TEST(converted[1] == L'b');
        BOOST_TEST(converted[2] == L'c');
    }
    {
        const auto converted = convert(src.data() + 3);
        BOOST_TEST(converted.size() == 1);
        BOOST_TEST(converted[0] == L'c');
    }
}

BOOST_AUTO_TEST_SUITE_END()
