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
    else if ('\n' == ch) {
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
cab::fixWrappedLines(const std::string &str)
{
  static const std::regex lineEndWithNoTag("\\n(?![a-z]+(-[a-z]+)*:)",
                                           std::regex::ECMAScript | std::regex::icase |
                                           std::regex::optimize);
  return std::regex_replace(str, lineEndWithNoTag, "");
}
