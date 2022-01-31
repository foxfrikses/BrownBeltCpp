#pragma once

#include "date.h"

class DatePeriod final {
  Date from_, to_;

public:
  constexpr DatePeriod() = default;
  constexpr DatePeriod(Date from, Date to) : from_(from), to_(to) {
    if (to < from) {
      throw std::runtime_error("Incorrect DatePeriod: to cannot be less than from");
    }
  }
  constexpr const Date& From() const {return from_;}
  constexpr const Date& To() const {return to_;}
};

std::istream& operator>>(std::istream& stream, DatePeriod& period);
