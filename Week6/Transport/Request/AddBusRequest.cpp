#include "AddBusRequest.h"

#include "RequestVisitor.h"

namespace Requests {
  AddBus::~AddBus() = default;
  
  void AddBus::Access(IRequestVisitor &v) { v.Visit(*this); }
}
