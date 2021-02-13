#include "gtest/gtest.h"
#include "stringreg.h"

TEST(CabrilloBasics,NewlineTests)
{
  {
    const std::string expectedResult="This is a test\nof an old newline convention\n";
    const std::string testSrcs[] {
      "This is a test\n\rof an old newline convention\n\r",
      "This is a test\r\nof an old newline convention\r\n",
      "This is a test\rof an old newline convention\r",
      expectedResult
    };
    for (const auto &str : testSrcs) {
      EXPECT_EQ(expectedResult, cab::translateeol(str));
    }
  }
  {
    const std::string expectedResult="\n\n\n\n\n";
    const std::string testSrcs[] {
      "\n\r\n\r\n\r\n\r\n\r",
      "\r\n\r\n\r\n\r\n\r\n",
      "\r\r\r\r\r",
       expectedResult
    };
    for (const auto &str : testSrcs) {
      EXPECT_EQ(expectedResult, cab::translateeol(str));
    }
  }
  
}
