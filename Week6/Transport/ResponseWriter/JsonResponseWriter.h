#pragma once

#include "ResponseWriter.h"

namespace ResponseWriter
{
  class Json : public IResponseWriter {
  public:
    void Write(std::ostream& stream, Responses responses) const override;
  };
}
