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

std::string removeSpaceBeforeTags(const std::string &str);

/**
 * @brief if there are overflow lines that wrapped
 *        try to fix them
 */
std::string fixWrappedLines(const std::string &str);

/**
 * @brief remove any X-QSO: tag lines
 */
std::string removeXQSOLines(const std::string &str);

/**
 * @brief trim leading and trailing whitespace from string
 */
std::string trim(const std::string &str);
}

#endif /*  __STRINGREG_H_LOADED__ */
