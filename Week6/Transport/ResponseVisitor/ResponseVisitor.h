#pragma once

#include "GetBusResponse.h"
#include "GetStopResponse.h"
#include "RouteResponse.h"

class IResponseVisitor {
public:
  virtual void Visit(Responses::GetBus& response) {}
  virtual void Visit(Responses::GetStop& response) {}
  virtual void Visit(Responses::Route& response) {}
};
