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
   * @brief Create an object to convert the multiline string @p text
   *        into an array of text fields based on the column structure
   *        of the text.
   * @param[in] text   a string with zero or more lines terminated by
   *                   newline characters. Expects no tab characters.
   */
  explicit TableText(const char *multilineText);

  /**
   * @brief Type used to hold the array of array of column fields.
   */
  using RowAndColumnList = std::vector<std::vector<std::string>>;

  /**
   * @brief Convert the lines of text into a collection of separated
   *        fields based on the whitespace of the table.
   */
  RowAndColumnList
  tabulate(unsigned minCols=0u) const;

  std::size_t getNumRows() const noexcept
  {
    return d_numRows;
  }

  std::size_t getMaxWidth() const noexcept
  {
    return d_spaceCounts.size();
  }
private:
  TableText() = delete;

  /// Count the number of spaces per column
  void
  countSpaces();

  /// Return all the unique space counts
  std::vector<int>
  uniqueSpaceCounts() const;

  /**
   * @brief The table text where each element of the std::vector is a line of text
   *
   * Each entry does not have a newline character at the end.
   */
  const std::string d_text;

  /**
   * @brief d_spaceCounts[i] holds the number of spaces in column i of all the text lines
   */
  std::vector<int> d_spaceCounts;

  /// The number of lines in the table
  std::size_t d_numRows;

  struct ColumnRange {
    std::size_t begin;          // the first column
    std::size_t end;            // one past the last column
  };

  void
  findColumns(const int                 minSpaceForColEnd,
              std::vector<ColumnRange> &table) const;

  RowAndColumnList::value_type
  fieldsFromLine(const std::vector<ColumnRange> &table,
                 const std::string &line) const;

  RowAndColumnList
  copyColumns(const std::vector<ColumnRange> &table) const;

};

}

#endif /*  __TABLETEXT_H_LOADED__ */
