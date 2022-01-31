#pragma once

#include "Response.h"
#include "RequestVisitor.h"
#include "TransportDirectory.h"

#include <vector>
#include <memory>

namespace RequestVisitor
{
  class ComputeStat : public IRequestVisitor {
    using ResponsePtr = std::unique_ptr<IResponse>;
    
    std::vector<ResponsePtr> responses;
    const TransportDirectory &directory;
  public:
    ComputeStat(const TransportDirectory &directory);
    ~ComputeStat() override;
    
    std::vector<ResponsePtr> ReleaseResponses();
    
    void Visit(Requests::GetBus& request) override;
    void Visit(Requests::GetStop& request) override;
    void Visit(Requests::Route& request) override;
  };
}

