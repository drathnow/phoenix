#include <gtest/gtest.h>
#include <StringHelper.h>
#include <string>

using namespace std;

namespace zios::foundation
{

TEST(StringHelperTest, shouldTrimSpacesFromRightEndOfStrings)
{
    string str;
    // right trim of space/whitespace
    string str1 = "  wer \tet  \t\r\n";
    str = StringHelper::rtrim(str1);
    EXPECT_EQ("  wer \tet", str);
    // right trim of whitespace/space
    str1 = "  wer \tet\t\r\n    ";
    str = StringHelper::rtrim(str1);
    EXPECT_EQ("  wer \tet", str);
}

TEST(StringHelperTest, shouldTrimSpacesFromLeftEndOfStrings)
{
    //ltrim of whitespace
    string str1 = "\t\r\nweret\t\r";
    string str = StringHelper::ltrim(str1);
    EXPECT_EQ("weret\t\r", str);

    //ltrim of space/whitespace
    str1 = "  \n\tweret\t\r";
    str = StringHelper::ltrim(str1);
    EXPECT_EQ("weret\t\r", str);
}

TEST(StringHelperTest, shouldTrimSpacesFromLeftAndRightEndOfStrings)
{
    // both end trim (leave whitespace in middle)
    string str1 = "\t \r\nwer\n et\t\r";
    string str = StringHelper::trim(str1);
    EXPECT_EQ("wer\n et", str);
}

}
