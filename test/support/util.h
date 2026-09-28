// Formatting library for C++ - test utilities
//
// Copyright (c) 2012 - present, Victor Zverovich and {fmt} contributors
// All rights reserved.
//
// For the license information refer to format.h.

#include <cstdarg>
#include <cstdio>

template <size_t SIZE>
void safe_sprintf(char (&buffer)[SIZE], const char* format, ...) {
  std::va_list args;
  va_start(args, format);
  vsnprintf(buffer, SIZE, format, args);
  va_end(args);
}

class date {
  int year_, month_, day_;

 public:
  date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

  auto year() const -> int { return year_; }
  auto month() const -> int { return month_; }
  auto day() const -> int { return day_; }
};
