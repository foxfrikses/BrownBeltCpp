#pragma once

#include "ResponseWriter.h"

namespace ResponseWriter
{
  class Stream : public IResponseWriter {
  public:
    void Write(std::ostream& stream, Responses responses) const override;
  };
}
