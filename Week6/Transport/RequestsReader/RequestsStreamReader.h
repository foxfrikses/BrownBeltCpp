#pragma once

#include "RequestsReader.h"

namespace RequestsReader
{
  class Stream : public IRequestsReader {
  public:
    Requests Read(std::istream& stream) const override;
  };
}

