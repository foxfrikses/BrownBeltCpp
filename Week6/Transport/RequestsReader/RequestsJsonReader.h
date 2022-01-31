#pragma once

#include "RequestsReader.h"

namespace RequestsReader
{
  class Json : public IRequestsReader {
  public:
    Requests Read(std::istream& stream) const override;
  };
}

