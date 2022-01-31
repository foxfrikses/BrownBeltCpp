#pragma once

#include "TransportDirectory.h"

#include "StopDistancesManager.h"

#include <vector>
#include <string>

using StopDistances = std::vector<std::pair<std::string, double>>;

class TransportDirectoryBuilder {

  StopDistancesManager distancesManager_;

  std::unordered_set<std::string> names_;
  std::unordered_map<std::string_view, StopInfo> stops_;
  std::unordered_map<std::string_view, BusInfo> buses_;

public:
  void AddBus(std::string name, std::vector<std::string> stops);
  void AddStop(std::string name, double latitude, double longitude);
  void AddStopDistanses(std::string stop, StopDistances distances);

  TransportDirectory GetTransportDirectory();
};
