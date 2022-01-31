#pragma once

#include <cmath>
#include <memory>
#include <algorithm>
#include <inttypes.h>

#include "budget_manager.h"
#include "summing_segment_tree.h"

class SegmTreeBudgetManager : public IBudgetManager {
  struct DailyBudget {
    double earned = 0.0;
    double spent = 0.0;
    
    operator double() const {return earned - spent;}
    friend DailyBudget operator+(const DailyBudget& lhs, const DailyBudget& rhs) {
      return {.earned = lhs.earned + rhs.earned, .spent = lhs.spent + rhs.spent};
    }
    friend DailyBudget operator*(const DailyBudget& lhs, const DailyBudget& rhs) {
      return {.earned = lhs.earned * rhs.earned, .spent = lhs.spent * rhs.spent};
    }
    friend DailyBudget operator/(const DailyBudget& lhs, const DailyBudget& rhs) {
      return {.earned = lhs.earned / rhs.earned, .spent = lhs.spent / rhs.spent};
    }
  };
  
  struct BulkMoneyAdder {
    DailyBudget delta;
  };
  
  struct BulkTaxApplier {
    DailyBudget factor {1.0, 1.0};
  };
  
  class BulkLinearUpdater {
  public:
    BulkLinearUpdater() = default;
    BulkLinearUpdater(const BulkMoneyAdder& add) : add_(add) {}
    BulkLinearUpdater(const BulkTaxApplier& tax) : tax_(tax) {}
    
    void CombineWith(const BulkLinearUpdater& other) {
      tax_.factor = tax_.factor * other.tax_.factor;
      add_.delta = add_.delta * other.tax_.factor + other.add_.delta;
    }
    
    DailyBudget Collapse(DailyBudget origin, IndexSegment segment) const {
      double length = segment.length();
      return origin * tax_.factor + add_.delta * DailyBudget{ .earned = length, .spent = length};
    }
  
  private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
  };
  
  class BudgetManager : public SummingSegmentTree<DailyBudget, BulkLinearUpdater> {
  public:
    BudgetManager(size_t size) : SummingSegmentTree(size) {}
  };
  
  BudgetManager manager;

public:
  SegmTreeBudgetManager() : manager(endDate - beginDate) {}
  
  double ComputeIncome(const DatePeriod& period) const override {
    return manager.ComputeSum(MakeDateSegment(period.From(), period.To()));
  }
  void Earn(const DatePeriod& period, double value) override {
    auto segment = MakeDateSegment(period.From(), period.To());
    auto operation = BulkMoneyAdder{ .delta.earned = value / segment.length(),
                                     .delta.spent = 0.0};
    manager.AddBulkOperation(segment, operation);
  }
  void PayTax(const DatePeriod& period, double percentage) override {
    auto segment = MakeDateSegment(period.From(), period.To());
    auto operation = BulkTaxApplier{.factor.earned = 1.0 - 0.01 * percentage,
                                    .factor.spent = 1.0};
    manager.AddBulkOperation(segment, operation);
  }
  void Spend(const DatePeriod& period, double value) override {
    auto segment = MakeDateSegment(period.From(), period.To());
    auto operation = BulkMoneyAdder{ .delta.earned = 0.0,
                                     .delta.spent = value / segment.length()};
    manager.AddBulkOperation(segment, operation);
  }

private:
  static size_t ComputeIndexFor(const Date& date) {
    return date - beginDate;
  }
  
  static IndexSegment MakeDateSegment(const Date& date_from, const Date& date_to) {
    return {ComputeIndexFor(date_from), ComputeIndexFor(date_to) + 1};
  }
};

