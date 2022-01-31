#pragma once

#include "Request.h"

#include <string>

namespace Requests
{
  class Route : public IRequest {
  public:
    ~Route () override;
    void Access(IRequestVisitor &v) override;
    
    std::string from;
    std::string to;
    int id = 0;
  };
}