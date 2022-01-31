#include "TransportDirectory.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <limits>

using namespace std;
using StopVector = StopDistancesManager::StopVector;

Graph::VertexId
TransportDirectory::SrcStopIdToVertexId(StopId stopId) const {
  return stopId;
}

Graph::VertexId
TransportDirectory::DstStopIdToVertexId(StopId stopId) const {
  return stopId + stops_.size();
}

TransportDirectory::StopId
TransportDirectory::VertexIdToDstStopId(Graph::VertexId vertexId) const {
  return vertexId % stops_.size();
}

TransportDirectory::TransportDirectory(unordered_set<string> names,
                                       unordered_map<string_view, StopInfo> stops,
                                       unordered_map<string_view, BusInfo> buses,
                                       StopDistancesManager stopDistancesManager,
                                       unique_ptr<RoutingSettings> routingSettings)
  : names_(move(names))
  , stopDistancesManager_(move(stopDistancesManager))
  , routingSettings_(move(routingSettings))
  , graph_(stops.size() * 2)
{
  if (!routingSettings_ ||
      routingSettings_->busWaitTime <= 0 ||
      routingSettings_->busVelocity <= 0) {
    throw runtime_error("routingSettings are uninitialized");
  }
  
  {
    size_t stopId = 0;
    stops_.resize(stops.size());
    for (auto& [stopName, stopInfo] : stops) {
      stops_[stopId] = move(stopInfo);
      stopNameToId_[stopName] = stopId;
      ++stopId;
    }
  }
  
  {
    size_t busId = 0;
    buses_.resize(buses.size());
    for (auto&[busName, busInfo] : buses) {
      buses_[busId] = move(busInfo);
      busNameToId_[busName] = busId;
      ++busId;
    }
  }

  {
    const double velocity = (routingSettings_->busVelocity * 1000 / 60);

    for (size_t busId = 0; busId < buses_.size(); ++busId) {
      auto& bus = buses_[busId];

      for (size_t srcStopIdx = 0; srcStopIdx < bus.stops.size() - 1; ++srcStopIdx) {
        double distance = 0.0;
        for (size_t dstStopIdx = srcStopIdx + 1; dstStopIdx < bus.stops.size(); ++dstStopIdx) {
          auto stopVector = StopVector{
              .src = bus.stops[dstStopIdx - 1],
              .dst = bus.stops[dstStopIdx]
          };
          distance += stopDistancesManager_.ComputeDistance(stopVector).real;

          edgeIdToBusEdgeInfo.emplace(
            graph_.GetEdgeCount(),
            BusEdgeInfo{.busId = busId, .stopCount = int(dstStopIdx - srcStopIdx)}
          );

          graph_.AddEdge(Graph::Edge<double>{
              .from = SrcStopIdToVertexId(stopNameToId_[bus.stops[srcStopIdx]]),
              .to = DstStopIdToVertexId(stopNameToId_[bus.stops[dstStopIdx]]),
              .weight = distance / velocity
          });
        }
      }
    }
  }

  {
    const double busWaitTime = routingSettings_->busWaitTime;

    for (size_t stopId = 0; stopId < stops_.size(); ++stopId) {

      auto& stop = stops_[stopId];

      edgeIdToStopId.emplace(graph_.GetEdgeCount(), stopId);

      graph_.AddEdge(Graph::Edge<double>{
          .from = DstStopIdToVertexId(stopId),
          .to = SrcStopIdToVertexId(stopId),
          .weight = busWaitTime
      });
    }


  }
  
  router_ = make_unique<Graph::Router<double>>(graph_);
}

std::optional<BusInfo> TransportDirectory::GetBusInfo(std::string_view busName) const {
  if (auto it = busNameToId_.find(busName); it != busNameToId_.end()) {
    return buses_[it->second];
  }

  return nullopt;
}

std::optional<StopInfo> TransportDirectory::GetStopInfo(std::string_view stopName) const {
  if (auto it = stopNameToId_.find(stopName); it != stopNameToId_.end()) {
    return stops_[it->second];
  }

  return nullopt;
}

std::optional<Route> TransportDirectory::GetRoute(const RouteRequest& routeRequest) const {
  std::optional<Route> route;
  
  auto from = stopNameToId_.find(routeRequest.from);
  auto to = stopNameToId_.find( routeRequest.to);
  if (from == stopNameToId_.end() || to == stopNameToId_.end()) {
    return route;
  }
  
  auto routeInfo = router_->BuildRoute(
    DstStopIdToVertexId(from->second),
    DstStopIdToVertexId(to->second)
  );
  if (!routeInfo) {
    return route;
  }
  
  route = Route();
  route->totalTime = routeInfo->weight;

  for (Graph::EdgeId edgeIdx = 0; edgeIdx < routeInfo->edge_count; ++edgeIdx) {
    auto edgeId = router_->GetRouteEdge(routeInfo->id, edgeIdx);

    if (auto etsIt = edgeIdToStopId.find(edgeId); etsIt != edgeIdToStopId.end()) {
      auto waitItem = make_unique<RouteItems::Wait>();
      waitItem->stopName = stops_[etsIt->second].stopName;
      waitItem->time = routingSettings_->busWaitTime;
      route->items.push_back(move(waitItem));
    }
    else if (auto etbIt = edgeIdToBusEdgeInfo.find(edgeId); etbIt != edgeIdToBusEdgeInfo.end()) {
      auto& edge = graph_.GetEdge(edgeId);
      auto& bus = buses_[etbIt->second.busId];

      auto busItem = make_unique<RouteItems::Bus>();
      busItem->bus = bus.busName;
      busItem->time = edge.weight;
      busItem->spanCount = etbIt->second.stopCount;

      route->items.push_back(move(busItem));
    }
  }

  router_->ReleaseRoute(routeInfo->id);
  
  return route;
}
