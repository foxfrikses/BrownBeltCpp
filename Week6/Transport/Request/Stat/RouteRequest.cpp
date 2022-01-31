#include "RouteRequest.h"

#include "RequestVisitor.h"

namespace Requests {
  Route::~Route() = default;
  
  void Route::Access(IRequestVisitor &v) { v.Visit(*this); }
}