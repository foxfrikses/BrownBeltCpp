#pragma once

#include <optional>
#include <vector>
#include <string>
#include <string_view>
#include <set>
#include <unordered_set>
#include <unordered_map>

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

class TransportDirectory {
public:
  friend class TransportDirectoryBuilder;
  
  std::optional<BusInfo> GetBusInfo(std::string_view busName) const;
  std::optional<StopInfo> GetStopInfo(std::string_view stopName) const;

private:
  std::unordered_set<std::string> names_;
  std::unordered_map<std::string_view, StopInfo> stops_;
  std::unordered_map<std::string_view, BusInfo> buses_;
  
  TransportDirectory(std::unordered_set<std::string> names,
                     std::unordered_map<std::string_view, StopInfo> stops,
                     std::unordered_map<std::string_view, BusInfo> buses);
};
