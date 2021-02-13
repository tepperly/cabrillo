#include "stringreg.h"

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
	result.append(1u,'\n');
	break;
      case 2:
	result.append(1u,'\n');
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
	result.append(1u,'\n');
	break;
      case 2:
	result.append(1u,'\n');
	break;
      }
    }
    else {
      if ((1 == state) || (2 == state)) {
	result.append(1u,'\n');
	state = 0;
      }
      result.append(1u,ch);
    }
  }
}