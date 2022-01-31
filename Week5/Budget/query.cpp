#include "query.h"

#include "query_visitor.h"

std::istream& operator>>(std::istream& stream, QueryType& type) {
  std::string typeString;
  stream >> typeString;
  if (typeString == "ComputeIncome") {
    type = QueryType::ComputeIncome;
  }
  else if (typeString == "Earn") {
    type = QueryType::Earn;
  }
  else if (typeString == "PayTax") {
    type = QueryType::PayTax;
  }
  else if (typeString == "Spend") {
    type = QueryType::Spend;
  }
  else {
    type = QueryType::None;
  }
  return stream;
}

#define ACCESS(CLASS_NAME) \
  void CLASS_NAME::Access(IQueryVisitor& visitor) const {visitor.Visit(*this);}
  
namespace Queries {
  ACCESS(ComputeIncome);
  ACCESS(Earn);
  ACCESS(PayTax);
  ACCESS(Spend);
}

std::unique_ptr<IQuery> ReadQuery(std::istream& stream) {
  QueryType type;
  stream >> type;
  
  switch (type) {
    case QueryType::ComputeIncome:
    {
      auto ptr = std::make_unique<Queries::ComputeIncome>();
      stream >> ptr->period;
      return std::move(ptr);
    }
    
    case QueryType::Earn:
    {
      auto ptr = std::make_unique<Queries::Earn>();
      stream >> ptr->period >> ptr->value;
      return std::move(ptr);
    }
    
    case QueryType::PayTax:
    {
      auto ptr = std::make_unique<Queries::PayTax>();
      stream >> ptr->period >> ptr->percentage;
      return std::move(ptr);
    }
    
    case QueryType::Spend:
    {
      auto ptr = std::make_unique<Queries::Spend>();
      stream >> ptr->period >> ptr->value;
      return std::move(ptr);
    }
    
    default:
      throw std::runtime_error("Incorrect QueryType");
  }
}