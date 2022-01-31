#pragma once

#include "ResponseVisitor.h"

#include "json.h"

namespace ResponseVisitor
{
  class WriteToJson : public IResponseVisitor {
    std::vector<Json::Node>& nodes;
  public:
    WriteToJson(std::vector<Json::Node>& nodes);
    
    void Visit(Responses::GetBus& response) override;
    void Visit(Responses::GetStop& response) override;
    void Visit(Responses::Route& response) override;
  };
}
