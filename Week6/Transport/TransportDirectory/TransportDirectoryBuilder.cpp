#include "TransportDirectoryBuilder.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

int GetUniqueStrNum(std::vector<std::string_view> strs) {
  std::sort(strs.begin(), strs.end());
  strs.erase(std::unique(strs.begin(), strs.end()), strs.end());
  return static_cast<int>(strs.size());
}

void TransportDirectoryBuilder::AddBus(std::string name, std::vector<std::string> stops) {
  BusInfo info;
  info.busName = *names_.insert(name).first;
  info.stops.reserve(stops.size());
  
  for (auto &stop : stops) {
    info.stops.emplace_back(*names_.insert(std::move(stop)).first);
  }

  buses_[info.busName] = std::move(info);
}

void TransportDirectoryBuilder::AddStop(std::string name, double latitude, double longitude) {
  StopInfo info;
  info.stopName = *names_.insert(name).first;

  stops_[info.stopName] = info;

  distancesManager_.SetCoords(info.stopName, {latitude, longitude});
}

void TransportDirectoryBuilder::AddStopDistanses(std::string stop, StopDistances distances) {
  StopDistancesManager::StopVector stops;
  stops.first = *names_.insert(std::move(stop)).first;
  
  for (auto& [stop, distance] : distances) {
    stops.second = *names_.insert(std::move(stop)).first;
    distancesManager_.SetDistance(stops, distance);
  }
}

TransportDirectory TransportDirectoryBuilder::GetTransportDirectory() {
  for (auto& [busName, busInfo] : buses_) {
    busInfo.uniqueStops = ::GetUniqueStrNum(busInfo.stops);
    auto distance = distancesManager_.ComputeDistance(busInfo.stops);
    busInfo.routeLength = distance.real;
    busInfo.curvature = distance.real / distance.ideal;

    for (auto stop : busInfo.stops) {
      stops_[stop].buses.insert(busName); 
    }
  }

  return {std::move(names_), std::move(stops_), std::move(buses_)};
}
