#pragma once

#include "RequestVisitor.h"
#include "json.h"

#include <iostream>

namespace RequestVisitor
{
  class ReadFromJson : public IRequestVisitor {
    const Json::Node& node;
  
  public:
    ReadFromJson(const Json::Node& node);
    ~ReadFromJson() override;
    
    void Visit(Requests::AddBus& request) override;
    void Visit(Requests::AddStop& request) override;
    
    void Visit(Requests::GetBus& request) override;
    void Visit(Requests::GetStop& request) override;
    void Visit(Requests::Route& request) override;
  };
}
