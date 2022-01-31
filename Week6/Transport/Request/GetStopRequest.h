#pragma once

#include "Request.h"

#include <string>

namespace Requests
{
  class GetStop : public IRequest {
  public:
    ~GetStop() override;
    void Access(IRequestVisitor &v) override;
    
    std::string name;
    int id = 0;
  };
}
