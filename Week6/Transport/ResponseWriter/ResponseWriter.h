#pragma once

#include <memory>
#include <ostream>
#include <vector>

#include "Response.h"

class IResponseWriter {
public:
  using ResponsePtr = std::unique_ptr<IResponse>;
  
  using Responses = std::vector<ResponsePtr>;
  
  virtual void Write(std::ostream& stream, Responses responses) const = 0;
};

