#pragma once

struct IQuery;

namespace Queries {
  struct ComputeIncome;
  struct Earn;
  struct PayTax;
  struct Spend;
}

class IQueryVisitor {
public:
  void VisitQuery(const IQuery& query);
  
  virtual void Visit(const Queries::ComputeIncome&) = 0;
  virtual void Visit(const Queries::PayTax&) = 0;
  virtual void Visit(const Queries::Earn&) = 0;
  virtual void Visit(const Queries::Spend&) = 0;
};