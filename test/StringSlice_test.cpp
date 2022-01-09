#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstring>

#include "StringSlice.h"

namespace string_slice_tests
{
    using namespace scottz0r;

    TEST_CASE("StringSlice_Constructors", )
    {
        SECTION("Default")
        {
            StringSlice ss;

            REQUIRE(ss.empty());
            REQUIRE(ss.data() == nullptr);
        }

        SECTION("Null terminated c string")
        {
            const char* data = "Test";
            StringSlice ss(data);
            REQUIRE(ss.data() == data);
            REQUIRE(ss.size() == 4);
        }

        SECTION("Pointer and size")
        {
            const char* data = "ASDF";
            StringSlice ss(data, 3);
            REQUIRE(ss.data() == data);
            REQUIRE(ss.size() == 3);
        }

        SECTION("Copy Constructor")
        {
            const char* data = "Test";
            StringSlice ss1(data);
            StringSlice ss2(ss1);

            REQUIRE(ss2.data() == ss1.data());
            REQUIRE(ss2.size() == ss1.size());
        }

        SECTION("Assignment")
        {
            // Technically not a constructor, but w/e.
            const char* data = "Test";
            StringSlice ss1(data);
            StringSlice ss2;
            ss2 = ss1;

            REQUIRE(ss2.data() == ss1.data());
            REQUIRE(ss2.size() == ss1.size());
        }
    }

    TEST_CASE("StringSilce_Simple_Methods")
    {
        SECTION("at")
        {
            StringSlice ss("ASDF");

            REQUIRE(ss.at(0) == 'A');
            REQUIRE(ss.at(1) == 'S');
            REQUIRE(ss.at(2) == 'D');
            REQUIRE(ss.at(3) == 'F');
            REQUIRE(ss.at(4) == -1);
            REQUIRE(ss.at(100) == -1);

            const char unicode[] = { (char)0xc3, (char)0x9c, 0 };
            StringSlice ss_uni(unicode);

            REQUIRE(ss_uni.at(0) == (char)0xc3);
            REQUIRE(ss_uni.at(1) == (char)0x9c);
            REQUIRE(ss_uni.at(2) == -1);
        }

        SECTION("data")
        {
            const char* data = "Test";
            StringSlice ss_empty;
            StringSlice ss_not_empty(data);

            REQUIRE(ss_empty.data() == nullptr);
            REQUIRE(ss_not_empty.data() == data);
        }

        SECTION("empty")
        {
            StringSlice ss_empty;
            StringSlice ss_not_empty("Test");

            REQUIRE(ss_empty.empty());
            REQUIRE_FALSE(ss_not_empty.empty());
        }

        SECTION("size")
        {
            const char* data = "Test";
            StringSlice ss_empty;
            StringSlice ss_not_empty(data);

            REQUIRE(ss_empty.size() == 0);
            REQUIRE(ss_not_empty.size() == 4);
        }

        SECTION("operator[]")
        {
            const char* data = "ASDFJKL;";
            StringSlice ss(data, 4);

            REQUIRE(ss[0] == 'A');
            REQUIRE(ss[1] == 'S');
            REQUIRE(ss[2] == 'D');
            REQUIRE(ss[3] == 'F');

            // Technically undefined, but the next bytes are known in this test. Should not check or error.
            REQUIRE(ss[4] == 'J');
        }
    }

    TEST_CASE("StringSlice_Compare")
    {
        SECTION("Equal")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2 = "Test";

            int res = ss1.compare(ss2);
            REQUIRE(res == 0);

            int res2 = ss2.compare(ss1);
            REQUIRE(res == 0);
        }

        SECTION("Less")
        {
            StringSlice ss1 = "Tea";
            StringSlice ss2 = "Test";
            StringSlice ss3 = "Test Two";

            int res = ss1.compare(ss2);
            REQUIRE(res < 0);

            int res2 = ss1.compare(ss3);
            REQUIRE(res2 < 0);
        }

        SECTION("Greater")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2 = "Bob";
            StringSlice ss3 = "Test Two";

            int res = ss1.compare(ss2);
            REQUIRE(res > 0);

            int res2 = ss3.compare(ss1);
            REQUIRE(res2 > 0);
        }

        SECTION("With empty")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2;

            int res = ss1.compare(ss2);
            REQUIRE(res > 0);

            int res2 = ss2.compare(ss1);
            REQUIRE(res2 < 0);
        }
    }

    TEST_CASE("StringSlice_copy_to")
    {
        SECTION("Good Case")
        {
            StringSlice ss("Bob");
            char buffer[64];

            auto r = ss.copy_to(buffer);

            REQUIRE(r == 3);
            REQUIRE(buffer[0] == 'B');
            REQUIRE(buffer[1] == 'o');
            REQUIRE(buffer[2] == 'b');
            REQUIRE(buffer[3] == 0);

        }

        SECTION("Buffer too small")
        {
            StringSlice ss("Bob Belcher");
            char buffer[4];

            auto r = ss.copy_to(buffer);

            REQUIRE(r == 3);
            REQUIRE(buffer[0] == 'B');
            REQUIRE(buffer[1] == 'o');
            REQUIRE(buffer[2] == 'b');
            REQUIRE(buffer[3] == 0);
        }

        SECTION("1 byte buffer")
        {
            StringSlice ss("Bob Belcher");
            char buffer[1];

            auto r = ss.copy_to(buffer);

            REQUIRE(r == 0);
            REQUIRE(buffer[0] == 0);
        }

        SECTION("Bad input")
        {
            StringSlice ss("Bob Belcher");
            auto r1 = ss.copy_to(nullptr, 123);
            REQUIRE(r1 == 0);

            char buffer[64];
            auto r2 = ss.copy_to(buffer, 0);
            REQUIRE(r2 == 0);
        }
    }

    TEST_CASE("StringSlice_Stripping")
    {
        SECTION("lstrip")
        {
            StringSlice ss;
            StringSlice stripped;

            // Right and left whitespace
            ss = "\r\nTest\t ";
            stripped = ss.lstrip();
            REQUIRE(stripped == "Test\t ");

            // Right whitespace
            ss = "TEST \r\n";
            stripped = ss.lstrip();
            REQUIRE(stripped == "TEST \r\n");

            // Left whitespace
            ss = "\r\nTEST";
            stripped = ss.lstrip();
            REQUIRE(stripped == "TEST");

            // All whitespace
            ss = "  \r\n";
            stripped = ss.lstrip();
            REQUIRE(stripped == "");

            // No whitespace
            ss = "AB";
            stripped = ss.lstrip();
            REQUIRE(stripped == "AB");

            // Empty string
            ss = "";
            stripped = ss.lstrip();
            REQUIRE(stripped == "");
        }

        SECTION("rstrip")
        {
            StringSlice ss;
            StringSlice stripped;

            // Right and left whitespace
            ss = "\r\nTest\t ";
            stripped = ss.rstrip();
            REQUIRE(stripped == "\r\nTest");

            // Right whitespace
            ss = "TEST \r\n";
            stripped = ss.rstrip();
            REQUIRE(stripped == "TEST");

            // Left whitespace
            ss = "\r\nTEST";
            stripped = ss.rstrip();
            REQUIRE(stripped == "\r\nTEST");

            // All whitespace
            ss = "  \r\n";
            stripped = ss.rstrip();
            REQUIRE(stripped == "");

            // No whitespace
            ss = "AB";
            stripped = ss.rstrip();
            REQUIRE(stripped == "AB");

            // Empty string
            ss = "";
            stripped = ss.rstrip();
            REQUIRE(stripped == "");
        }

        SECTION("strip")
        {
            StringSlice ss;
            StringSlice stripped;

            // Right and left whitespace
            ss = "\r\nTest\t ";
            stripped = ss.strip();
            REQUIRE(stripped == "Test");

            // Right whitespace
            ss = "TEST \r\n";
            stripped = ss.strip();
            REQUIRE(stripped == "TEST");

            // Left whitespace
            ss = "\r\nTEST";
            stripped = ss.strip();
            REQUIRE(stripped == "TEST");

            // All whitespace
            ss = "  \r\n";
            stripped = ss.strip();
            REQUIRE(stripped == "");

            // No whitespace
            ss = "AB";
            stripped = ss.strip();
            REQUIRE(stripped == "AB");

            // Empty string
            ss = "";
            stripped = ss.strip();
            REQUIRE(stripped == "");
        }
    }

    TEST_CASE("StringSlice_Find")
    {
        SECTION("Find Char")
        {
            StringSlice ss("Test Stuff");
            auto pos = ss.find(' ');
            REQUIRE(pos == 4);

            auto pos_nope = ss.find('Z');
            REQUIRE(pos_nope == StringSlice::npos);
        }
    }

    TEST_CASE("StringSlice_Substr")
    {
        SECTION("With Size")
        {
            StringSlice ss("Test String");
            auto sub = ss.substr(5, 6);
            REQUIRE(sub == "String");
        }

        SECTION("Size larger than slice")
        {
            StringSlice ss("Test String");
            auto sub = ss.substr(5, 200);
            REQUIRE(sub == "String");
        }

        SECTION("No length")
        {
            StringSlice ss("Test String");
            auto sub = ss.substr(2);
            REQUIRE(sub == "st String");
        }

        SECTION("Start out of range")
        {
            StringSlice ss("Test String");
            auto sub = ss.substr(200);
            REQUIRE(sub == "");
        }

        SECTION("On empty slice")
        {
            StringSlice ss;
            auto sub = ss.substr(1, 2);
            REQUIRE(sub.empty());
        }
    }

    TEST_CASE("Comparison_Operators")
    {
        SECTION("equals")
        {
            StringSlice ss1("Test");
            StringSlice ss2("Test Two", 4);
            StringSlice ss3;

            REQUIRE(ss1 == ss2);
            REQUIRE(ss2 == ss1);
            REQUIRE(ss1 == "Test");
            REQUIRE_FALSE(ss1 == ss3);
            REQUIRE_FALSE(ss2 == ss3);
        }

        SECTION("not equals")
        {
            StringSlice ss1("Test");
            StringSlice ss2("Test Two", 4);
            StringSlice ss3;

            REQUIRE_FALSE(ss1 != ss2);
            REQUIRE_FALSE(ss2 != ss1);
            REQUIRE_FALSE(ss1 != "Test");
            REQUIRE(ss1 != ss3);
            REQUIRE(ss2 != ss3);
        }

        SECTION("less than")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2 = "Bob";
            StringSlice ss3 = "Test Two";
            StringSlice ss4;

            REQUIRE(ss1 < ss3);
            REQUIRE(ss2 < ss1);
            REQUIRE_FALSE(ss3 < ss1);
            REQUIRE_FALSE(ss1 < ss2);
            REQUIRE_FALSE(ss1 < ss4);

            REQUIRE_FALSE(ss1 < ss1);
        }

        SECTION("less than equal")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2 = "Bob";
            StringSlice ss3 = "Test Two";
            StringSlice ss4;

            REQUIRE(ss1 <= ss3);
            REQUIRE(ss2 <= ss1);
            REQUIRE_FALSE(ss3 <= ss1);
            REQUIRE_FALSE(ss1 <= ss2);
            REQUIRE_FALSE(ss1 <= ss4);

            REQUIRE(ss1 <= ss1);
        }

        SECTION("greater than")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2 = "Bob";
            StringSlice ss3 = "Test Two";
            StringSlice ss4;

            REQUIRE_FALSE(ss1 > ss3);
            REQUIRE_FALSE(ss2 > ss1);
            REQUIRE(ss3 > ss1);
            REQUIRE(ss1 > ss2);
            REQUIRE(ss1 > ss4);

            REQUIRE_FALSE(ss1 > ss1);
        }

        SECTION("greater than equal")
        {
            StringSlice ss1 = "Test";
            StringSlice ss2 = "Bob";
            StringSlice ss3 = "Test Two";
            StringSlice ss4;

            REQUIRE_FALSE(ss1 >= ss3);
            REQUIRE_FALSE(ss2 >= ss1);
            REQUIRE(ss3 >= ss1);
            REQUIRE(ss1 >= ss2);
            REQUIRE(ss1 >= ss4);

            REQUIRE(ss1 >= ss1);
        }
    }

    TEST_CASE("bool operator")
    {
        StringSlice ss;
        REQUIRE_FALSE((bool)ss);

        ss = "test";
        REQUIRE((bool)ss);
    }

    TEST_CASE("not operator")
    {
        StringSlice ss;
        REQUIRE(!ss);

        ss = "test";
        REQUIRE_FALSE(!ss);
    }

    TEST_CASE("to_slice")
    {
        SECTION("String literal")
        {
            char buffer[] = "TEST";
            auto ss = to_slice(buffer);
            REQUIRE(ss.size() == 4);
            REQUIRE(ss[0] == 'T');
            REQUIRE(ss == "TEST");
        }

        SECTION("Empty string")
        {
            char buffer[] = "";
            auto ss = to_slice(buffer);
            REQUIRE(ss.size() == 0);
            REQUIRE(ss.empty());
        }
    }

    TEST_CASE("get_line")
    {
        SECTION("Single use")
        {
            StringSlice ss("test\r\nstuff");

            auto line = get_line(ss);
            REQUIRE(line == "test\r\n");
        }

        SECTION("No new line")
        {
            StringSlice ss("test stuff");

            auto line = get_line(ss);
            REQUIRE(line == "test stuff");
        }

        SECTION("Multiple lines")
        {
            const char* data = "test\r\nstuff\r\nyep\r\n";
            StringSlice buffer = data;
            StringSlice line;

            line = get_line(buffer);
            buffer = buffer.substr(line.size());
            REQUIRE(line == "test\r\n");

            line = get_line(buffer);
            buffer = buffer.substr(line.size());
            REQUIRE(line == "stuff\r\n");

            line = get_line(buffer);
            buffer = buffer.substr(line.size());
            REQUIRE(line == "yep\r\n");

            line = get_line(buffer);
            buffer = buffer.substr(line.size());
            REQUIRE(line.empty());
        }
    }
}
