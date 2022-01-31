#include "ResponseWriteToJsonVisitor.h"

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
}