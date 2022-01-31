#pragma once

#include "AddBusRequest.h"
#include "AddStopRequest.h"

#include "GetBusRequest.h"
#include "GetStopRequest.h"

#include <stdexcept>

class IRequestVisitor {
public:
  virtual ~IRequestVisitor() = default;
  
  virtual void Visit(Requests::AddBus& request) {throw std::runtime_error("Visit AddBus not overridden");}
  virtual void Visit(Requests::AddStop& request) {throw std::runtime_error("Visit AddStop not overridden");}
  virtual void Visit(Requests::GetBus& request) {throw std::runtime_error("Visit GetBus not overridden");}
  virtual void Visit(Requests::GetStop& request) {throw std::runtime_error("Visit GetStop not overridden");}
};
