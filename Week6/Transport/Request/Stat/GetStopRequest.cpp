#include "GetStopRequest.h"

#include "RequestVisitor.h"

namespace Requests {
  GetStop::~GetStop() = default;
  
  void GetStop::Access(IRequestVisitor &v) { v.Visit(*this); }
}