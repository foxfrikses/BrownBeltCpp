#pragma once

#include <memory>
#include <iostream>
#include <utility>

#include "date_period.h"

class IQueryVisitor;

enum class QueryType {
  None,
  ComputeIncome,
  Earn,
  PayTax,
  Spend
};

std::istream& operator>>(std::istream& stream, QueryType& type);

struct IQuery {
  virtual void Access(IQueryVisitor& visitor) const = 0;
};

namespace Queries {
  struct ComputeIncome final : IQuery {
  public:
    void Access(IQueryVisitor& visitor) const override;
    
    DatePeriod period;
  };
  
  struct Earn final : IQuery {
  public:
    void Access(IQueryVisitor& visitor) const override;
    
    DatePeriod period;
    double value = 0.0;
  };
  
  struct PayTax final : IQuery {
  public:
    void Access(IQueryVisitor& visitor) const override;
  
    DatePeriod period;
    double percentage = 0.0;
  };
  
  struct Spend final : IQuery {
  public:
    void Access(IQueryVisitor& visitor) const override;
  
    DatePeriod period;
    double value = 0.0;
  };
}

std::unique_ptr<IQuery> ReadQuery(std::istream& stream);

