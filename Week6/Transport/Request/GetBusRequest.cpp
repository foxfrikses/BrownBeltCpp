#include "GetBusRequest.h"

#include "RequestVisitor.h"

namespace Requests {
  GetBus::~GetBus() = default;
  
  void GetBus::Access(IRequestVisitor &v) { v.Visit(*this); }
}