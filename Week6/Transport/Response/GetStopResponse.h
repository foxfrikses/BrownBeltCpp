#pragma once

#include "Response.h"

#include <optional>
#include <vector>
#include <string>
#include <string_view>

namespace Responses {
  class GetStop : public IResponse {
  public:
    ~GetStop() override = default;
    void Access(IResponseVisitor& v) override;
    
    struct Data {
      std::vector<std::string> buses;
    };
    
    std::string stopName;
    int requestId = 0;
    
    std::optional<Data> data;
  };
}
