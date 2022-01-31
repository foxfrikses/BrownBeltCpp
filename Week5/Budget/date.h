#pragma once

#include <stdexcept>
#include <iostream>
#include <tuple>

#include "int_type.h"

INT_TYPE(Day);
INT_TYPE(Month);
INT_TYPE(Year);

class Date final {
public:
  constexpr Date();
  constexpr Date(Day day, Month month, Year year);
  
  constexpr auto GetDay() const {return day_;}
  constexpr auto GetMonth() const {return month_;}
  constexpr auto GetYear() const {return year_;}
  
  constexpr int AsDaystamp() const;
  
  friend constexpr bool operator<(const Date&, const Date&);

protected:
  Day day_;
  Month month_;
  Year year_;
};

constexpr int DayCountPerMonth(Month month, Year year) {
  switch (month) {
    case 1: return 31;
    case 2: return 28 + (year % 4 == 0 ? 1 : 0);
    case 3: return 31;
    case 4: return 30;
    case 5: return 31;
    case 6: return 30;
    case 7: return 31;
    case 8: return 31;
    case 9: return 30;
    case 10: return 31;
    case 11: return 30;
    case 12: return 31;
    default:
      throw std::runtime_error("Incorrect Month");
  }
}

constexpr int Date::AsDaystamp() const {
  int days = (year_ - 1900) * 365 + ((year_ - 1900) + 3) / 4;
  for (int month = 1; month  < month_; ++month) {
    days += DayCountPerMonth(Month(month), year_);
  }
  days += day_ - 1;
  return days;
}

constexpr Date::Date()
  : day_(1)
  , month_(1)
  , year_(2000)
{}

constexpr bool IsWithinInterval(int val, int leftBound, int rightBound) {
  return val >= leftBound && val <= rightBound;
}

constexpr Date::Date(Day day, Month month, Year year)
  : day_(day)
  , month_(month)
  , year_(year)
{
  if (!IsWithinInterval(day_, 1, DayCountPerMonth(month_, year_)) ||
      !IsWithinInterval(month_, 1, 12))
  {
    throw std::runtime_error("Incorrect date");
  }
}

constexpr int operator-(const Date& lhs, const Date& rhs) {
  return (lhs.AsDaystamp() - rhs.AsDaystamp());
}

constexpr bool operator<(const Date& lhs, const Date& rhs) {
  return
      std::tie(lhs.year_, lhs.month_, lhs.day_) <
      std::tie(rhs.year_, rhs.month_, rhs.day_);
}

std::istream& operator>>(std::istream& stream, Date& rhs);
