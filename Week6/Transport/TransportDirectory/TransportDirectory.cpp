#include "TransportDirectory.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

TransportDirectory::TransportDirectory(unordered_set<string> names,
                                       unordered_map<string_view, StopInfo> stops,
                                       unordered_map<string_view, BusInfo> buses)
  : names_(move(names))
  , stops_(move(stops))
  , buses_(move(buses))
{}

std::optional<BusInfo> TransportDirectory::GetBusInfo(std::string_view busName) const {
  if (auto it = buses_.find(busName); it != buses_.end()) {
    return it->second;
  }

  return nullopt;
}

std::optional<StopInfo> TransportDirectory::GetStopInfo(std::string_view stopName) const {
  if (auto it = stops_.find(stopName); it != stops_.end()) {
    return it->second;
  }

  return nullopt;
}
