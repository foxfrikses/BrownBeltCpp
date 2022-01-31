#pragma once

#include "Request.h"

#include <vector>
#include <string>

namespace Requests {
  class AddBus: public IRequest {
  public:
    ~AddBus() override;
    
    void Access(IRequestVisitor &v) override;
    
    std::string name;
    std::vector<std::string> stopList;
  };
}