/**
 * @file stringreg.h
 * @brief String regularization
 */

#ifndef __STRINGREG_H_LOADED__
#define __STRINGREG_H_LOADED__
#include <string>

namespace cab {

/**
 * @brief convert the end of line convention to only newline
 */
std::string translateeol(const std::string &str);
}

#endif /*  __STRINGREG_H_LOADED__ */