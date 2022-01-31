#include "RequestComputeStatVisitor.h"

#include "GetBusResponse.h"
#include "GetStopResponse.h"

#include "GetBusRequest.h"
#include "GetStopRequest.h"

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
}
