#pragma once

#include "Request.h"
#include "RoutingSettings.h"

#include <vector>
#include <memory>
#include <istream>

class IRequestsReader {
public:
  using RequestPtr = std::unique_ptr<IRequest>;

  struct Requests {
    std::vector<RequestPtr> base;
    std::vector<RequestPtr> stat;
    std::unique_ptr<RoutingSettings> routingSettings;
  };

  virtual Requests Read(std::istream& stream) const = 0;
};

