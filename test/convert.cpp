// Copyright (c) 2020 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "winapi-utf8" project.
// For details, see https://github.com/egor-tensin/winapi-utf8.
// Distributed under the MIT License.

#include <winapi/utf8.hpp>

#include <boost/format.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace std {

ostream& operator<<(ostream& os, unsigned char c) {
    return os << boost::format("%|1$02x|") % static_cast<unsigned int>(c);
}

ostream& operator<<(ostream& os, const vector<unsigned char>& cs) {
    for (auto c : cs) {
        os << c;
    }
    return os;
}

}

namespace {

std::vector<unsigned char> from(std::initializer_list<unsigned char> xs) {
    return {xs};
}

template <typename CharT>
std::vector<unsigned char> from(const std::basic_string<CharT>& s) {
    const auto buf = reinterpret_cast<const unsigned char*>(s.c_str());
    const auto size = s.size() * sizeof(CharT);
    return {buf, buf + size};
}

std::vector<std::vector<unsigned char>> utf16 = {
    // Hello
    from({0x48, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x6f, 0x00}),
    // Привет
    from({0x1f, 0x04, 0x40, 0x04, 0x38, 0x04, 0x32, 0x04, 0x35, 0x04, 0x42, 0x04}),
};

std::vector<std::vector<unsigned char>> utf8 = {
    // Hello
    from({0x48, 0x65, 0x6c, 0x6c, 0x6f}),
    // Привет
    from({0xd0, 0x9f, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5, 0xd1, 0x82}),
};

}

BOOST_TEST_SPECIALIZED_COLLECTION_COMPARE(std::vector<unsigned char>);

BOOST_AUTO_TEST_SUITE(convert_tests)

BOOST_DATA_TEST_CASE(test_narrow,
                     boost::unit_test::data::make(utf16) ^ utf8,
                     input,
                     expected) {
    auto actual = from(winapi::narrow(input));
    BOOST_TEST(actual == expected, "Expected: " << expected << ", actual: " << actual);
}

BOOST_DATA_TEST_CASE(test_widen,
                     boost::unit_test::data::make(utf8) ^ utf16,
                     input,
                     expected) {
    auto actual = from(winapi::widen(input));
    BOOST_TEST(actual == expected, "Expected: " << expected << ", actual: " << actual);
}

BOOST_AUTO_TEST_SUITE_END()
