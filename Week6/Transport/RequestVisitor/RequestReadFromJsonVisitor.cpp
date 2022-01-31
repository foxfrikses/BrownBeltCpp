#include "RequestReadFromJsonVisitor.h"

#include <tuple>
#include <sstream>

using namespace std;

namespace RequestVisitor
{
  ReadFromJson::ReadFromJson(const Json::Node &node)
    : node(node)
  {}
  
  ReadFromJson::~ReadFromJson() = default;
  
  void ReadFromJson::Visit(Requests::AddBus &request) {
    request.name = node.AsMap().at("name").AsString();
  
    bool is_roundtrip = node.AsMap().at("is_roundtrip").AsBool();
    auto &stops = node.AsMap().at("stops").AsArray();
  
    request.stopList.clear();
    request.stopList.reserve(!is_roundtrip ? stops.size() * 2 - 1 : stops.size());
  
    for (auto &stop: stops) {
      request.stopList.push_back(stop.AsString());
    }
  
    if (!is_roundtrip) {
      for (size_t i = request.stopList.size() - 1; i > 0; --i) {
        request.stopList.push_back(request.stopList[i - 1]);
      }
    }
  }
  
  void ReadFromJson::Visit(Requests::AddStop &request) {
    request.name = node.AsMap().at("name").AsString();
    request.latitude = node.AsMap().at("latitude").AsDouble();
    request.longitude = node.AsMap().at("longitude").AsDouble();
  
    auto &road_distances = node.AsMap().at("road_distances").AsMap();
    request.distances.clear();
    request.distances.reserve(road_distances.size());
    for (auto &[stop, dist]: road_distances) {
      request.distances.emplace_back(stop, dist.AsDouble());
    }
  }
  
  void ReadFromJson::Visit(Requests::GetBus& request) {
    request.name = node.AsMap().at("name").AsString();
    request.id = node.AsMap().at("id").AsInt();
  }
  
  void ReadFromJson::Visit(Requests::GetStop& request) {
    request.name = node.AsMap().at("name").AsString();
    request.id = node.AsMap().at("id").AsInt();
  }
}