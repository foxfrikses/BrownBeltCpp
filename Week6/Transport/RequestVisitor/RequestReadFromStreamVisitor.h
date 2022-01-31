#pragma once

#include "RequestVisitor.h"

#include <iostream>

namespace RequestVisitor
{
  class ReadFromStream : public IRequestVisitor {
    std::istream& stream;
  public:
    ReadFromStream(std::istream &stream = std::cin);
    ~ReadFromStream() override;
    
    void Visit(Requests::AddBus& request) override;
    void Visit(Requests::AddStop& request) override;
    void Visit(Requests::GetBus& request) override;
    void Visit(Requests::GetStop& request) override;
  };
};
