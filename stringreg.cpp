#include "stringreg.h"
#include <regex>

std::string
cab::translateeol(const std::string &str)
{
  std::string result;
  result.reserve(str.length());
  short state(0);
  for (const char ch : str) {
    if ('\r' == ch) {
      switch(state) {
      case 0:
        state = 1;
        break;
      case 1:
        result.push_back('\n');
        break;
      case 2:
        result.push_back('\n');
        state = 0;
        break;
      }
    }
    else
      if ('\n' == ch) {
        switch(state) {
        case 0:
          state = 2;
          break;
        case 1: // \r\n -> \n
          state = 0;
          result.push_back('\n');
          break;
        case 2:
          result.push_back('\n');
          break;
        }
      }
      else {
        if ((1 == state) || (2 == state)) {
          result.push_back('\n');
          state = 0;
        }
        result.push_back(ch);
      }
  }
  if ((1 == state) || (2 == state)) {
    result.push_back('\n');
  }
  return result;
}

std::string
cab::removeSpaceBeforeTags(const std::string &str)
{
  static const std::regex leadingSpace("(^|\n)[ \\t]+([a-z]+(-[a-z]+)*:)",
                                       std::regex::ECMAScript | std::regex::icase |
                                       std::regex::optimize);
  return std::regex_replace(str, leadingSpace, "$1$2");
}

std::string
cab::removeXQSOLines(const std::string &str)
{
  static const std::regex xqsoLine("(^|\n)x-qso:.*\n",
                                   std::regex::ECMAScript | std::regex::icase |
                                   std::regex::optimize);
  return std::regex_replace(str, xqsoLine, "$1");

}

std::string
cab::fixWrappedLines(const std::string &str)
{
  static const std::regex lineEndWithNoTag("\\n(?![a-z]+(-[a-z]+)*:)",
      std::regex::ECMAScript | std::regex::icase |
      std::regex::optimize);
  return std::regex_replace(str, lineEndWithNoTag, "");
}

std::string
cab::trim(const std::string &str)
{
  static const std::string whitespace(" \t\n\r\f\v");
  const std::size_t start(str.find_first_not_of(whitespace));
  if (std::string::npos != start) {
    const std::size_t end(str.find_last_not_of(whitespace));
    if (std::string::npos != end) {
      return str.substr(start, 1u+end-start);
    }
  }
  // otherwise it's all space
  return "";
}
