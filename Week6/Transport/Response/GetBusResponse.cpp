#include "GetBusResponse.h"

#include "ResponseVisitor.h"

namespace Responses
{
  void GetBus::Access(IResponseVisitor &v) {
    v.Visit(*this);
  }
}
