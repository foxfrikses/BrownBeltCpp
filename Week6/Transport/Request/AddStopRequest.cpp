#include "AddStopRequest.h"

#include "RequestVisitor.h"

namespace Requests {
  AddStop::~AddStop() = default;
  
  void AddStop::Access(IRequestVisitor &v) { v.Visit(*this); }
  
}