#include "ComputeStatRequestVisitor.h"

#include "GetBusResponse.h"
#include "GetStopResponse.h"
#include "RouteResponse.h"

#include "GetBusRequest.h"
#include "GetStopRequest.h"
#include "RouteRequest.h"

using namespace std;

namespace RequestVisitor
{
  ComputeStat::ComputeStat(const TransportDirectory &directory)
      : directory(directory)
  {}
  
  ComputeStat::~ComputeStat() = default;
  
  std::vector<ComputeStat::ResponsePtr>
  ComputeStat::ReleaseResponses() {
    return std::move(responses);
  }
  
  void ComputeStat::Visit(Requests::GetBus &request) {
    const auto infoOpt = directory.GetBusInfo(request.name);
    
    auto response = std::make_unique<Responses::GetBus>();
    response->requestId = request.id;
    response->busName = request.name;
  
    if (infoOpt.has_value()) {
      auto &info = infoOpt.value();
  
      response->data = Responses::GetBus::Data();
      response->data->routeLength = info.routeLength;
      response->data->stopsOnRoute = info.stops.size();
      response->data->uniqueStops = info.uniqueStops;
      response->data->curvature = info.curvature;
    }
    
    responses.push_back(move(response));
  }
  
  void ComputeStat::Visit(Requests::GetStop &request) {
    const auto infoOpt = directory.GetStopInfo(request.name);
    
    auto response = make_unique<Responses::GetStop>();
    response->requestId = request.id;
    response->stopName = request.name;
  
    if (infoOpt.has_value()) {
      auto &info = infoOpt.value();
      
      response->data = Responses::GetStop::Data();
      response->data->buses.assign(info.buses.begin(), info.buses.end());
    }
    
    responses.push_back(move(response));
  }
  
  void ComputeStat::Visit(Requests::Route& request) {
    auto route = directory.GetRoute(RouteRequest{.from = request.from, .to = request.to});
    
    auto response = make_unique<Responses::Route>();
    response->requestId = request.id;
    
    if (route) {
      response->data = Responses::Route::Data();
      
      response->data->totalTime = route->totalTime;
      auto& items = response->data->items;
      
      for (auto& routeItem : route->items)  {
        switch (routeItem->type) {
          case RouteItem::Type::Bus:
          {
            auto& bus = static_cast<RouteItems::Bus&>(*routeItem);
            
            auto busItem = make_unique<Responses::RouteItems::Bus>();
            busItem->bus = bus.bus;
            busItem->spanCount = bus.spanCount;
            busItem->time = bus.time;
            items.push_back(move(busItem));
            break;
          }
          case RouteItem::Type::Wait:
          {
            auto& wait = static_cast<RouteItems::Wait&>(*routeItem);
  
            auto waitItem = make_unique<Responses::RouteItems::Wait>();
            waitItem->stopName = wait.stopName;
            waitItem->time = wait.time;
            items.push_back(move(waitItem));
            break;
          }
          default:
            throw runtime_error("Unexpected routeItem::Type");
        }
      }
    }
  
    responses.push_back(move(response));
  }
}
