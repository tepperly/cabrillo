#include "tabletext.h"

#include <algorithm>
#include <cstring>

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
