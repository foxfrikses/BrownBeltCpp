#include "GetStopResponse.h"

#include "ResponseVisitor.h"

namespace Responses
{
  void GetStop::Access(IResponseVisitor &v) {
    v.Visit(*this);
  }
}
