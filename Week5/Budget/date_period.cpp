#pragma once

#include "date_period.h"

std::istream& operator>>(std::istream& stream, DatePeriod& period) {
  Date from, to;
  stream >> from >> to;
  period = DatePeriod(from, to);
  return stream;
}
