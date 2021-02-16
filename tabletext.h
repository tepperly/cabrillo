/**
 * @file   tabletext.h
 * @brief  Define a class to convert column oriented tables in plain text into
 *         delineated fields
 */
#ifndef __TABLETEXT_H_LOADED__
#define __TABLETEXT_H_LOADED__
#include <vector>
#include <string>

namespace cab {

class TableText {
public:
  /**
   * @brief Create an object to convert the multiline string @p text
   *        into an array of text fields based on the column structure
   *        of the text.
   * @param[in] text   a string with zero or more lines terminated by
   *                   newline characters. Expects no tab characters.
   */
  explicit TableText(const std::string &text);

  /**
   * @brief Create an object to convert the lines of text in @p lines.
   * @param[in] lines  an ordered collection of text lines with no
   *                   newline or tab characters
   */
  explicit TableText(const std::vector<std::string> &lines);

  /**
   * @brief Create an object to convert the multiline string @p text
   *        into an array of text fields based on the column structure
   *        of the text.
   * @param[in] text   a string with zero or more lines terminated by
   *                   newline characters. Expects no tab characters.
   */
  explicit TableText(const char *multilineText);

  std::size_t getNumRows() const noexcept
  {
    return d_textLines.size();
  }

  std::size_t getMaxWidth() const noexcept
  {
    return d_maxWidth;
  }
private:
  TableText() = delete;

  /// Count the number of spaces per column
  void
  countSpaces();

  /// Add the string defined by [begin, end) to the list of lines
  void
  addSubstring(const char *const begin, const char *const end);

  /**
   * @brief The table text where each element of the std::vector is a line of text
   *
   * Each entry does not have a newline character at the end.
   */
  std::vector<std::string> d_textLines;

  /**
   * @brief d_spaceCounts[i] holds the number of spaces in column i of all the text lines
   */
  std::vector<int> d_spaceCounts;

  /// The maximum number of characters in any particular line
  std::size_t d_maxWidth;

};

}

#endif /*  __TABLETEXT_H_LOADED__ */
