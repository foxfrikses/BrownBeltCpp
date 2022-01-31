#include "TransportDirectoryRouteItems.h"

RouteItem::RouteItem(Type type) : type(type) {}

namespace RouteItems {
  Wait::Wait() : RouteItem(RouteItem::Type::Wait) {}
  
  Bus::Bus() : RouteItem(RouteItem::Type::Bus) {}
}