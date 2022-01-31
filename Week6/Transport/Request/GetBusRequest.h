#pragma once

#include "Request.h"

#include <string>

namespace Requests {
  class GetBus : public IRequest {
  public:
    ~GetBus() override;
    
    void Access(IRequestVisitor &v) override;
    
    std::string name;
    int id = 0;
  };
}
