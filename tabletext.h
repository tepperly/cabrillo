/**
 * @file   tabletext.h
 * @brief  Define a class to convert column oriented tables in plain text into
 *         delineated fields
 *
 * The section of the code addresses the challenge of finding columns
 * in a multi-line text string. The assumption is that the block of
 * text contains information in columns that are typically separated
 * by blocks of space. Occasionally, there can be spaces inside a
 * column. For example, the text "NEW JERSEY" has an embedded space,
 * but it doesn't mean there is a column break between "NEW" and
 * "JERSEY". There can also be situations (hopefully few) where
 * the text from one column bleeds another.
 *
 * Because of the variations in space mentioned above, identifying
 * columns is not a simple cut-and-dried algorithm. It involves
 * looking at the statistics of the text and making reasonable
 * choices. The approach here is based on counting the number of
 * spaces in each column for all the lines of text. Column starts are
 * indicated by a decrease in the number of spaces from column i to
 * j. A column end is indicated by columns that are all space or
 * nearly so.
 *
 * Ham radio contest logs is the application area for this particular
 * class, so it's tuned to make reasonable decisions for what's normal
 * in those datasets. For example, most of the columns in a Cabrillo
 * file are left justified. The notable exceptions are the frequency
 * and serial numbers that are right justified. This means that the
 * start of a column should be indicated by a sharp decrease in the
 * number of spaces in a column. At the end of a column, the number
 * of spaces may gradually increase.
 *
 * Most contests define abbreviations to avoid spaces inside column
 * values (e.g., NJ instead of "NEW JERSEY"), but in practice, people
 * submit logs that are not necessarily up to the published
 * specifications.
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
   *                   newline characters.
   *                   This assumes that the only space characters
   *                   in the string are space (e.g., ASCII 32) and
   *                   newline (e.g., ASCII 10). There should be no
   *                   horizontal tab, vertical tab, carriage returns,
   *                   form feeds, or non-ASCII characters.
   */
  explicit TableText(const std::string &text);

  /**
   * @brief Create an object to convert the multiline string @p text
   *        into an array of text fields based on the column structure
   *        of the text.
   * @param[in] text   a string with zero or more lines terminated by
   *                   newline characters.
   *                   This assumes that the only space characters
   *                   in the string are space (e.g., ASCII 32) and
   *                   newline (e.g., ASCII 10). There should be no
   *                   horizontal tab, vertical tab, carriage returns,
   *                   form feeds, or non-ASCII characters.
   */
  explicit TableText(const char *multilineText);

  /**
   * @brief Type used to hold the array of array of column fields.
   */
  using RowAndColumnList = std::vector<std::vector<std::string>>;

  /**
   * @brief Convert the lines of text into a collection of separated
   *        fields based on the whitespace of the table.
   * @param minCols  indicates that a minimum number of columns
   *                 is expected in the text lines. Having a minimum
   *                 of zero, will result in the smallest number
   *                 of columns
   * @exception std::out_of_range  this exception indicates
   * that the algorithm could not identify @p minCols or more
   * columns.
   */
  RowAndColumnList
  tabulate(unsigned minCols=0u) const;

  /**
   * @brief return the number of lines in the text.
   */
  std::size_t getNumRows() const noexcept
  {
    return d_numRows;
  }

  /**
   * @brief return the number of columns in the widest line of text
   */
  std::size_t getMaxWidth() const noexcept
  {
    return d_spaceCounts.size();
  }
private:
  /// One cannot construct this object without the text.
  TableText() = delete;

  /**
   * @brief Count the number of spaces in each column of the text.
   *
   * The algorithm used to identify columns in the text block
   * is based on counting the number of spaces per column in the text.
   * This routine efficiently counts the number of spaces per column
   * in the text treating short lines as though they were padded with
   * spaces at the end.
   */
  void
  countSpaces();

  /// Return a sorted list of all the unique space counts in the
  /// vector of space counts per column. The returned vector is sorted
  /// from smallest to largest element, and it always includes zero.
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

  /// An internal type used to hold the beginning and ending of a column
  struct ColumnRange {
    std::size_t begin;          // the first column
    std::size_t end;            // one past the last column
  };

  /**
   * @brief find all the columns in the text assuming that a brief
   *        between columns must have at least @p minSpaceForColEnd
   *        spaces
   * @parm minSpaceForColEnd  the minimum number of spaces that
   *                          indicaters a break between columns
   * @param[out] table        a vector to hold the column definitions
   *                          on output
   */
  void
  findColumns(const int                 minSpaceForColEnd,
              std::vector<ColumnRange> &table) const;

  /**
   * @brief convert a single line of text into a list of
   *        column string values
   * @param table  indicates the starting and ending position
   *               each each table column.
   * @param line   the line of text
   */
  RowAndColumnList::value_type
  fieldsFromLine(const std::vector<ColumnRange> &table,
                 const std::string &line) const;

  /**
   * @brief convert all the lines of text into a collection
   *        of collections of fields.
   * @param table   the positions that define each table column
   */
  RowAndColumnList
  copyColumns(const std::vector<ColumnRange> &table) const;

};

}

#endif /*  __TABLETEXT_H_LOADED__ */
