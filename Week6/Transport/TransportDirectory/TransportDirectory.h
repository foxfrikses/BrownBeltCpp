#pragma once

#include <optional>
#include <vector>
#include <string>
#include <string_view>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <memory>

#include "StopDistancesManager.h"
#include "RoutingSettings.h"
#include "graph.h"
#include "router.h"
#include "TransportDirectoryRouteItems.h"

struct BusInfo {
  std::string_view busName;
  std::vector<std::string_view> stops;
  int uniqueStops = 0;
  double routeLength = 0.0;
  double curvature = 0.0;
};

struct StopInfo {
  std::string_view stopName;
  std::set<std::string_view> buses;
};

struct RouteRequest {
  std::string_view from;
  std::string_view to;
};

struct Route {
  double totalTime = 0.0;
  std::vector<std::unique_ptr<RouteItem>> items;
};

class TransportDirectory {
public:
  friend class TransportDirectoryBuilder;
  
  std::optional<BusInfo> GetBusInfo(std::string_view busName) const;
  std::optional<StopInfo> GetStopInfo(std::string_view stopName) const;
  std::optional<Route> GetRoute(const RouteRequest& routeRequest) const;

private:
  using StopId = size_t;

  std::unordered_set<std::string> names_;
  std::unordered_map<std::string_view, size_t> stopNameToId_;
  std::unordered_map<std::string_view, size_t> busNameToId_;
  
  std::vector<StopInfo> stops_;
  std::vector<BusInfo> buses_;

  struct BusEdgeInfo {
    size_t busId;
    int stopCount;
  };
  std::unordered_map<Graph::EdgeId, BusEdgeInfo> edgeIdToBusEdgeInfo;
  std::unordered_map<Graph::EdgeId, size_t> edgeIdToStopId;

  Graph::VertexId SrcStopIdToVertexId(StopId) const;
  Graph::VertexId DstStopIdToVertexId(StopId) const;
  StopId VertexIdToDstStopId(Graph::VertexId) const;

  StopDistancesManager stopDistancesManager_;
  std::unique_ptr<RoutingSettings> routingSettings_;
  
  Graph::DirectedWeightedGraph<double> graph_;
  std::unique_ptr<Graph::Router<double>> router_;
  
  TransportDirectory(std::unordered_set<std::string> names,
                     std::unordered_map<std::string_view, StopInfo> stops,
                     std::unordered_map<std::string_view, BusInfo> buses,
                     StopDistancesManager stopDistancesManager,
                     std::unique_ptr<RoutingSettings> routingSettings);
};
