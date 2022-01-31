#include "WriteToJsonResponseVisitor.h"

#include <map>
#include <string>

using Json::Node;
using namespace std;

namespace ResponseVisitor {
  
  WriteToJson::WriteToJson(vector<Node> &nodes) : nodes(nodes) {}
  
  void WriteToJson::Visit(Responses::GetBus& response) {
    map <string, Node> node{
        {"request_id", Node(response.requestId)}
    };
  
    if (!response.data) {
      node.emplace("error_message", "not found");
    }
    else {
      node.emplace("route_length",      Node(response.data->routeLength));
      node.emplace("curvature",         Node(response.data->curvature));
      node.emplace("stop_count",        Node(response.data->stopsOnRoute));
      node.emplace("unique_stop_count", Node(response.data->uniqueStops));
      node.emplace("request_id",        Node(response.requestId));
    }
    
    nodes.emplace_back(move(node));
  }
  
  void WriteToJson::Visit(Responses::GetStop& response) {
    map <string, Node> node{
        {"request_id", Node(response.requestId)}
    };
    
    if (!response.data) {
      node.emplace("error_message", "not found");
    } else {
      vector<Node> buses;
      buses.reserve(response.data->buses.size());
      for (auto &bus: response.data->buses) {
        buses.emplace_back(move(bus));
      }
      node.emplace("buses", move(buses));
    }
    
    nodes.emplace_back(move(node));
  }
  
  void WriteToJson::Visit(Responses::Route& response) {
    map <string, Node> node{
        {"request_id", Node(response.requestId)}
    };
    
    if (!response.data) {
      node.emplace("error_message", "not found");
    }
    else {
      node.emplace("total_time", response.data->totalTime);
  
      vector<Node> items;
      
      for (auto& item : response.data->items) {
        using namespace Responses::RouteItems;
        
        switch (item->type) {
          case Item::Type::Wait:
          {
            auto& wait = static_cast<Wait&>(*item);
            
            map <string, Node> waitItem {
                {"type", Node("Wait")},
                {"stop_name", Node(wait.stopName)},
                {"time", Node(wait.time)}
            };
            
            items.push_back(move(waitItem));
            
            break;
          }
          case Item::Type::Bus:
          {
            auto& bus = static_cast<Bus&>(*item);
  
            map <string, Node> busItem {
                {"type", Node("Bus")},
                {"bus", Node(bus.bus)},
                {"span_count", Node(bus.spanCount)},
                {"time", Node(bus.time)}
            };
  
            items.push_back(move(busItem ));
  
            break;
          }
          default:
            throw std::runtime_error("Item type is not recognized");
        }
      }
      
      node.emplace("items", move(items));
    }
    nodes.emplace_back(move(node));
  }
}