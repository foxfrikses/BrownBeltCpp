#include "query_visitor.h"

#include "query.h"

void IQueryVisitor::VisitQuery(const IQuery& query) {
  query.Access(*this);
}