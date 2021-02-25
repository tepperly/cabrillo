#include "tabletext.h"
#include "stringreg.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

using namespace cab;

TableText::TableText(const std::string &multilineText)
  : d_text(multilineText),
    d_numRows(0u)
{
  countSpaces();
}

TableText::TableText(const char *multilineText)
  : d_text(multilineText),
    d_numRows(0u)
{
  countSpaces();
}

namespace {
/// short lines are treated like they are padded with spaces at the end
void
padShortLines(std::vector<int> &spaces, std::size_t curCol)
{
  while (curCol < spaces.size()) {
    ++spaces[curCol];
    ++curCol;
  }
}
}

void
TableText::countSpaces()
{
  std::size_t curCol(0u);
  d_spaceCounts.reserve(128u);
  for(const char ch : d_text) { // iterate through whole string
    if ('\n' == ch) {
      padShortLines(d_spaceCounts, curCol);
      // increment row count and reset for next line
      ++d_numRows;
      curCol = 0u;
    }
    else {
      if (curCol >= d_spaceCounts.size()) {
        d_spaceCounts.resize(curCol+1, d_numRows);
      }
      if (' ' == ch) {
        ++d_spaceCounts[curCol];
      }
      ++curCol;
    }
  }
  if (!d_text.empty() && ('\n' != d_text.back())) {
    ++d_numRows;
  }
}

std::vector<std::vector<std::string>>
                                   TableText::tabulate(unsigned minCols) const
{
  std::vector<int> spaces(d_spaceCounts);
  spaces.push_back(0);
  std::sort(spaces.begin(), spaces.end());
  auto it = std::unique(spaces.begin(), spaces.end());
  spaces.resize(std::distance(spaces.begin(), it));
  std::vector<ColumnRange> columns;
  columns.reserve(minCols);
  for(auto rit=spaces.rbegin(); rit != spaces.rend(); ++rit) {
    const int minSpaceForColEnd(*rit);
    findColumns(minSpaceForColEnd, columns);
    if (columns.size() >= minCols) {
      return copyColumns(columns);
    }
  }
  throw std::out_of_range("Unable to find enough columns");
}

void
TableText::findColumns(const int                minSpaceForColEnd,
                       std::vector<ColumnRange> &table) const
{
  const std::size_t maxWidth(d_spaceCounts.size());
  const int numLines(static_cast<int>(d_numRows));
  const int startColumnThreshold((3*numLines)/4);
  table.clear();
  std::size_t pos=0;
  while (pos < maxWidth) {
    ColumnRange cr;
    // find start
    while ((pos < maxWidth) && (d_spaceCounts[pos] >= numLines)) {
      ++pos;
    }
    if (pos < maxWidth) {
      int maxSpaces(startColumnThreshold);
      cr.begin = pos;
      // find end
      while (pos < maxWidth) {
        for(++pos; (pos < maxWidth) && (d_spaceCounts[pos] < minSpaceForColEnd); ++pos) {
          if (d_spaceCounts[pos] > maxSpaces) {
            maxSpaces = d_spaceCounts[pos];
          }
        }
        if ((pos + 1) < maxWidth) {
          if ((d_spaceCounts[pos] >= numLines) && (d_spaceCounts[pos+1] >= numLines)) {
            // two consecutive columns of all spaces => column must be done
            break;
          }
          ++pos;
          if (d_spaceCounts[pos] < maxSpaces) {
            // pos + 1 looks like the start of another column
            break;
          }
        }
        else {
          if ((pos < maxWidth) &&
              ((d_spaceCounts[pos] <= startColumnThreshold)  ||
               (d_spaceCounts[pos] == numLines))) {
            break;
            ++pos;
          }
        }
      }
      cr.end = pos;
      table.push_back(cr);
    }
  }
}

TableText::RowAndColumnList::value_type
TableText::fieldsFromLine(const std::vector<ColumnRange> &table,
                          const std::string &line) const
{
  std::vector<std::string> columnList;
  columnList.reserve(table.size());
  for(const ColumnRange &cr : table) {
    if (cr.begin < line.size()) {
      std::string newCol(cab::trim(line.substr(cr.begin, cr.end-cr.begin)));
      columnList.push_back(newCol);
    }
    else {
      columnList.push_back(std::string());
    }
  }
  return columnList;
}

TableText::RowAndColumnList
TableText::copyColumns(const std::vector<ColumnRange> &table) const
{
  std::vector<std::vector<std::string>> result;
  result.reserve(d_numRows);
  std::size_t cur(0u), next;
  while (std::string::npos != (next = d_text.find('\n', cur))) {
    result.push_back(std::move(fieldsFromLine(table, d_text.substr(cur, next-cur))));
    cur = next+1;
  }
  if (cur < d_text.size()) {
    result.push_back(std::move(fieldsFromLine(table, d_text.substr(cur))));
  }
  return result;
}
