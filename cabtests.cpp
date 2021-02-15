#include "gtest/gtest.h"
#include "stringreg.h"

TEST(CabrilloBasics,NewlineTests)
{
  {
    static const std::string expectedResult="This is a test\nof an old newline convention\n";
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
    static const std::string expectedResult="\n\n\n\n\n";
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

TEST(CabrilloBasics,SpaceLeadingTags)
{
  static const std::string expectedResults=
    "START-OF-LOG: 2.0\nARRL-SECTION: EB\nCALLSIGN: W1AW\nX-CQP-CALLSIGN: W1AW\nQSO:\nEND-OF-LOG:\n";
  const std::string input=
    " START-OF-LOG: 2.0\n  ARRL-SECTION: EB\n      CALLSIGN: W1AW\n \t   X-CQP-CALLSIGN: W1AW\n  QSO:\n     END-OF-LOG:\n";
  EXPECT_EQ(expectedResults,cab::removeSpaceBeforeTags(input));
}

TEST(CabrilloBasics, fixWrappedLines)
{
  static const std::string expectedResults=
    "NAME: Tom Epperly\nCATEGORY: SINGLE-OP ALL HIGH SSB\nADDRESS: Livermore, CA\nX-CQP-CALLSIGN: NS6T\nQSO: 14332 PH 2020-10-03 1601 NS6T          0001 TULA  K3AFZ         0001 PA    \nQSO: 14332 PH 2020-10-03 1601 NS6T          0002 TULA  K4FTU         0001 NC    ";
    const std::string testSrcs[] {
      expectedResults,                                
      "NAME: Tom \nEpperly\nCATEGORY: SINGLE-OP ALL HIGH SSB\nADDRESS: Livermore, CA\nX-CQP-CALLSIGN: NS6T\nQSO: 14332 PH 2020-10-03 1601 NS6T          0001 TULA  K3AFZ         0001 \nPA    \nQSO: 14332 PH 2020-10-03 1601 NS6T          0002 TULA  K4FTU         0001\n NC    \n",
      "NAME: Tom \nEpperly\nCATEGORY: SINGLE-OP A\nLL HIGH SSB\nADDRESS: Livermore\n, CA\nX-CQP-CALLSIGN: NS6T\nQSO: 14332 PH 2020-10-03 1601 NS6T          0001 TULA\n  K3AFZ         0001 \nPA    \nQSO: 14332 PH 2020-10-03 1601 NS6T          0002 TULA \n K4FTU         0001\n NC    \n"
    };
    for(const auto &str : testSrcs) {
      EXPECT_EQ(cab::fixWrappedLines(str), expectedResults);
    }
}
