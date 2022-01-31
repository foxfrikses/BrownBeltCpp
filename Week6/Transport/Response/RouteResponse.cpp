#include "RouteResponse.h"

#include "ResponseVisitor.h"

namespace Responses
{
  void Route::Access(IResponseVisitor &v) {
    v.Visit(*this);
  }
  
  namespace RouteItems
  {
    Item::Item(Type type) : type(type) {}
  
    Wait::Wait() : Item(Item::Type::Wait) {}
  
    Bus::Bus() : Item(Item::Type::Bus) {}
  }
}
