#pragma once

#include <string_view>

class RouteItem {
public:
  const enum class Type {Wait, Bus} type;
  
  RouteItem(Type type);
};

namespace RouteItems {
  class Wait : public RouteItem {
  public:
    Wait();
    
    std::string_view stopName;
    int time = 0;
  };
  
  class Bus : public RouteItem {
  public:
    Bus();
    
    std::string_view bus;
    int spanCount = 0;
    double time = 0.0;
  };
}