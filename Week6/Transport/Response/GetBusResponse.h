#pragma once

#include "Response.h"

#include <string>
#include <optional>

namespace Responses
{
  class GetBus : public IResponse {
  public:
    ~GetBus() override = default;
    void Access(IResponseVisitor& v) override;
    
    struct Data {
      int stopsOnRoute = 0;
      int uniqueStops = 0;
      double routeLength = 0.0;
      double curvature = 0.0;
    };
    
    std::string busName;
    int requestId = 0;
    
    std::optional<Data> data;
  };
}
