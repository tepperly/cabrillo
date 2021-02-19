#include "tabletext.h"
#include "stringreg.h"

#include <algorithm>
#include <cstring>
#include <stdexcept>

using namespace cab;

TableText::TableText(const std::vector<std::string> &lines)
  : d_textLines(lines), d_maxWidth(0u)
{
  for(const auto &line : lines) {
    if (line.length() > d_maxWidth) {
      d_maxWidth = line.length();
    }
  }
  countSpaces();
}

TableText::TableText(const std::string &multilineText)
  : d_maxWidth(0u)
{
  std::size_t cur(0u), next;
  while (std::string::npos != (next = multilineText.find('\n', cur))) {
    addSubstring(&(multilineText[cur]), &(multilineText[next]));
    cur = next + 1;
  }
  const std::size_t len(multilineText.length());
  if (cur < len) {
    addSubstring(&(multilineText[cur]), &(multilineText[len]));
  }
  countSpaces();
}

TableText::TableText(const char *multilineText)
  : d_maxWidth(0u)
{
  const char *cur=multilineText, *next;
  while (nullptr != (next = std::strchr(cur, '\n'))) {
    addSubstring(cur, next);
    cur = next + 1;
  }
  const std::size_t len(std::strlen(cur));
  if (len > 0u) {
    addSubstring(cur, cur+len);
  }
  countSpaces();
}

void
TableText::addSubstring(const char *const begin, const char *const end)
{
  d_textLines.emplace_back(begin, end);
  const std::size_t count(static_cast<std::size_t>(end-begin));
  if (count > d_maxWidth) {
    d_maxWidth = count;
  }
}

void
TableText::countSpaces()
{
  std::fill(d_spaceCounts.begin(),
            d_spaceCounts.begin()+std::min(d_maxWidth, d_spaceCounts.size()),
            0);
  d_spaceCounts.resize(d_maxWidth,0);
  for(const auto &line : d_textLines) {
    std::size_t i(0u);
    for(const char ch : line) {
      if (' ' == ch) {
        ++(d_spaceCounts[i]);
      }
      ++i;
    }
    // lines shorter than d_maxWidth are tested as though they are
    // padded with spaces
    while (i < d_maxWidth) {
      ++(d_spaceCounts[i]);
      ++i;
    }
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
  const int numLines(static_cast<int>(d_textLines.size()));
  const int startColumnThreshold((3*numLines)/4);
  table.clear();
  std::size_t pos=0;
  while (pos < d_maxWidth) {
    ColumnRange cr;
    // find start
    while ((pos < d_maxWidth) && (d_spaceCounts[pos] >= numLines)) {
      ++pos;
    }
    if (pos < d_maxWidth) {
      int maxSpaces(startColumnThreshold);
      cr.begin = pos;
      // find end
      while (pos < d_maxWidth) {
        for(++pos; (pos < d_maxWidth) && (d_spaceCounts[pos] < minSpaceForColEnd); ++pos) {
          if (d_spaceCounts[pos] > maxSpaces) {
            maxSpaces = d_spaceCounts[pos];
          }
        }
        if ((pos + 1) < d_maxWidth) {
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
          if ((d_spaceCounts[pos] <= startColumnThreshold)  ||
              (d_spaceCounts[pos] == numLines)) {
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

std::vector<std::vector<std::string>>
                                   TableText::copyColumns(const std::vector<ColumnRange> &table) const
{
  std::vector<std::vector<std::string>> result;
  result.reserve(d_textLines.size());
  for(const std::string line : d_textLines) {
    result.push_back(std::vector<std::string> {});
    std::vector<std::string> &columnList(result.back());
    for(const ColumnRange &cr : table) {
      if (cr.begin < line.size()) {
        std::string newCol(cab::trim(line.substr(cr.begin, cr.end-cr.begin)));
        columnList.push_back(newCol);
      }
      else {
        columnList.push_back(std::string());
      }
    }
  }
  return std::move(result);
}
