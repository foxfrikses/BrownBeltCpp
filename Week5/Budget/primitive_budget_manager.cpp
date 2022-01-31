#include "primitive_budget_manager.h"

double BudgetManager::DailyBudget::Total() const {return earned - spent;}

BudgetManager::BudgetManager() : budgetPerDay_(endDate - beginDate) {}

double BudgetManager::ComputeIncome(const DatePeriod& period) const {
  auto range = GetRangeFor(period);
  return std::accumulate(range.begin(), range.end(), 0.0,
                         [](double prev, const DailyBudget &budget) { return budget.Total() + prev; });
}

void BudgetManager::Earn(const DatePeriod& period, double value) {
  value /= (period.To() - period.From() + 1);
  for (auto &val: GetRangeFor(period)) {
    val.earned += value;
  }
}

void BudgetManager::PayTax(const DatePeriod& period, double percentage) {
  for (auto &val: GetRangeFor(period)) val.earned *= (1.0 - percentage * 0.01);
}

void BudgetManager::Spend(const DatePeriod& period, double value) {
  value /= (period.To() - period.From() + 1);
  for (auto &val: GetRangeFor(period)) val.spent += value;
}

Range<BudgetManager::BudgetContainer::const_iterator>
BudgetManager::GetRangeFor(const DatePeriod& period) const {
  return Range(
      budgetPerDay_.begin() + ComputeIndexFor(period.From()),
      budgetPerDay_.begin() + ComputeIndexFor(period.To()) + 1
  );
}

Range<BudgetManager::BudgetContainer::iterator>
BudgetManager::GetRangeFor(const DatePeriod& period) {
  return Range(
      budgetPerDay_.begin() + ComputeIndexFor(period.From()),
      budgetPerDay_.begin() + ComputeIndexFor(period.To()) + 1
  );
}

size_t BudgetManager::ComputeIndexFor(const Date& date) {
  if (date < beginDate || !(date < endDate)) {
    throw std::runtime_error("Incorrect date");
  }
  return static_cast<size_t>(date - beginDate);
}
