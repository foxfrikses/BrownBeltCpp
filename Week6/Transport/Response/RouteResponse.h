#pragma once

#include "Response.h"

#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace Responses {
  namespace RouteItems {
    class Item;
    class Wait;
    class Bus;
  }
  
  class Route : public IResponse {
  public:
    ~Route() override = default;
    void Access(IResponseVisitor& v) override;
    
    struct Data {
      std::vector<std::unique_ptr<RouteItems::Item>> items;
      double totalTime = 0.0;
    };
    
    std::optional<Data> data;
    int requestId = 0;
  };
  
  namespace RouteItems {
    class Item {
    public:
      const enum class Type {Wait, Bus} type;
      
      Item(Type type);
    };
    
    class Wait : public Item {
    public:
      Wait();
      
      std::string stopName;
      int time = 0;
    };
    
    class Bus : public Item {
    public:
      Bus();
      
      std::string bus;
      int spanCount = 0;
      double time = 0.0;
    };
  }
}
