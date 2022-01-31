#pragma once

#include <numeric>
#include <vector>

#include "budget_manager.h"
#include "range.h"

class BudgetManager final : public IBudgetManager {
public:
  BudgetManager();
  
  auto ComputeIncome(const DatePeriod& period) const -> double override;
  void Earn(const DatePeriod& period, double value) override;
  void PayTax(const DatePeriod& period, double percentage) override;
  void Spend(const DatePeriod& period, double value) override;

private:
  static size_t ComputeIndexFor(const Date& date);
  
  struct DailyBudget {
    double earned = 0.0;
    double spent = 0.0;
    
    double Total() const;
  };
  
  using BudgetContainer = std::vector<DailyBudget>;
  
  Range<BudgetContainer::const_iterator> GetRangeFor(const DatePeriod& period) const;
  Range<BudgetContainer::iterator> GetRangeFor(const DatePeriod& period);
  
  BudgetContainer budgetPerDay_;
};
