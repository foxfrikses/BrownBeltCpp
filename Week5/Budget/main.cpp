#include <iostream>
#include <vector>
#include <memory>

#include "range.h"
//#include "primitive_budget_manager.h"
#include "segm_tree_budget_manager.h"

#include "query.h"
#include "query_visitor.h"

namespace Responses {
  class ComputeIncome;
}

class IResponseVisitor;
struct IResponse {
  virtual void Access(IResponseVisitor& visitor) const = 0;
};

class IResponseVisitor {
public:
  void VisitResponse(const IResponse& response) {response.Access(*this);}

  virtual void Visit(const Responses::ComputeIncome&) = 0;
};

namespace Responses {
#define ACCESS_RESPONSE_VISITOR void Access(IResponseVisitor& visitor) const override {visitor.Visit(*this);}

  struct ComputeIncome final : IResponse {
    double value;
    ACCESS_RESPONSE_VISITOR;
  };
}

class ProcessQueryVisitor final : public IQueryVisitor {
  IBudgetManager& manager;
  std::vector<std::unique_ptr<IResponse>> responses;

public:
  ProcessQueryVisitor(IBudgetManager& manager)
    : manager(manager)
  {}

  auto ReleaseResponses() {return std::move(responses);}

  void Visit(const Queries::ComputeIncome& query) override {
    auto amount = manager.ComputeIncome(query.period);
    auto response = std::make_unique<Responses::ComputeIncome>();
    response->value = amount;
    responses.push_back(move(response));
  }
  void Visit(const Queries::PayTax& query) override {
    manager.PayTax(query.period, query.percentage);
  }
  void Visit(const Queries::Earn& query) override {
    manager.Earn(query.period, query.value);
  }
  void Visit(const Queries::Spend& query) override {
    manager.Spend(query.period, query.value);
  }
};

class PrintResponseVisitor final : public IResponseVisitor {
  std::ostream& stream;

public:
  PrintResponseVisitor(std::ostream& stream) : stream(stream) {}

  void Visit(const Responses::ComputeIncome& response) override {
    stream << response.value;
  }
};

int main() {
  std::cout.precision(25);

  SegmTreeBudgetManager manager;

  int queryCount;
  std::cin >> queryCount;

  std::vector<std::unique_ptr<IQuery>> queries(queryCount);
  for (auto& query : queries) {
    query = ReadQuery(std::cin);
  }

  ProcessQueryVisitor processQueryVisitor(manager);
  for (auto& query : queries) {
    processQueryVisitor.VisitQuery(*query);
  }

  PrintResponseVisitor printResponsesVisitor(std::cout);
  for (auto& response : processQueryVisitor.ReleaseResponses()) {
    printResponsesVisitor.VisitResponse(*response);
    std::cout << std::endl;
  }

  return 0;
}
