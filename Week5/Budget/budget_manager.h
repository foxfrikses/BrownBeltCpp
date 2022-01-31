#pragma once

#include "date_period.h"

class IBudgetManager {
public:
  static constexpr Date beginDate = Date(Day(1), Month(1), Year(2000));
  static constexpr Date endDate = Date(Day(1), Month(1), Year(2100));
  
  virtual double ComputeIncome(const DatePeriod& period) const = 0;
  virtual void Earn(const DatePeriod& period, double value) = 0;
  virtual void PayTax(const DatePeriod& period, double percentage) = 0;
  virtual void Spend(const DatePeriod& period, double value) = 0;
};
