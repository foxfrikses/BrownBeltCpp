#pragma once

#include "ResponseVisitor.h"

#include <iostream>

namespace ResponseVisitor
{
  class WriteToStream : public IResponseVisitor {
    std::ostream &stream;
  public:
    WriteToStream (std::ostream &stream = std::cout);
    
    void Visit(Responses::GetBus &response) override;
    
    void Visit(Responses::GetStop &response) override;
  };
}