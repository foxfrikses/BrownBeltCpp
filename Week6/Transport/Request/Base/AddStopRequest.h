#pragma once

#include "Request.h"

#include <vector>
#include <string>

namespace Requests {
  class AddStop : public IRequest {
  public:
    ~AddStop() override;
    
    void Access(IRequestVisitor &v) override;
    
    std::vector<std::pair<std::string, double>> distances;
    std::string name;
    double latitude = 0.0;
    double longitude = 0.0;
  };
}