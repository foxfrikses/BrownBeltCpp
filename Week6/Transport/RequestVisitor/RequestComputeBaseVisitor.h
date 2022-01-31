#pragma once

#include "Response.h"
#include "RequestVisitor.h"
#include "TransportDirectoryBuilder.h"

#include <vector>
#include <memory>

namespace RequestVisitor
{
  class ComputeBase : public IRequestVisitor
  {
    TransportDirectoryBuilder& builder_;
  public:
    ComputeBase(TransportDirectoryBuilder& builder);
    ~ComputeBase() override;
    
    void Visit(Requests::AddBus& request) override;
    
    void Visit(Requests::AddStop &request) override;
  };
}