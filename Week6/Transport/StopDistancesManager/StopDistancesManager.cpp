#include "StopDistancesManager.h"

#include <stdexcept>
#include <cmath>

using namespace std;

Distance& Distance::operator+=(const Distance &other) {
  ideal += other.ideal;
  real += other.real;
  return *this;
}

double DistanceBetween(Coords fst, Coords scnd) {
  static const double earthRadiusM = 6371.0 * 1000.0;
  static const double Pi_180 = 3.1415926535 / 180.0; 

  fst.latitude *= Pi_180;
  scnd.latitude *= Pi_180;
  fst.longitude *= Pi_180;
  scnd.longitude *= Pi_180;

  const double dLat = scnd.latitude - fst.latitude;
  const double dLon = scnd.longitude - fst.longitude;

  const double a = std::sin(dLat/2) * std::sin(dLat/2) +
    std::sin(dLon/2) * std::sin(dLon/2) * 
    std::cos(fst.latitude) * std::cos(scnd.latitude); 

  return earthRadiusM * 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
}

StopDistancesManager::StopDistancesManager(StopDistancesManager&& other)
  : coords_(move(other.coords_))
  , distances_(move(other.distances_))
{}

StopDistancesManager& StopDistancesManager::operator=(StopDistancesManager&& other) {
  coords_ = std::move(other.coords_);
  distances_ = std::move(other.distances_);
  return *this;
}

void StopDistancesManager::SetCoords(Stop stop, Coords coords) {
  coords_[stop] = coords;
}

void StopDistancesManager::SetDistance(StopVector stopVector, double distance) {
  distances_[stopVector].real = distance;
}

Distance StopDistancesManager::ComputeDistance(StopVector stopVector) {
  auto& distance = distances_.emplace(stopVector, Distance()).first->second;

  if (distance.ideal == 0.0 || distance.real == 0.0) {

    auto& distanceRev = distances_.emplace(
      StopVector {.src = stopVector.dst, .dst = stopVector.src},
      Distance {}
    ).first->second;

    if (distance.ideal == 0.0 || distanceRev.ideal == 0.0) {

      auto srcCoordsIt = coords_.find(stopVector.src);
      auto dstCoordsIt = coords_.find(stopVector.dst);

      if (srcCoordsIt == coords_.end() || dstCoordsIt == coords_.end()) {
        throw runtime_error("Cannot compute distance. Stop is not found");
      }

      distance.ideal = distanceRev.ideal =
        ::DistanceBetween(srcCoordsIt->second, dstCoordsIt->second);
    }

    if (distance.real == 0.0 || distanceRev.real == 0.0) {
      if (distance.real != 0.0) {
        distanceRev.real = distance.real;
      }
      else if (distanceRev.real != 0.0) {
        distance.real = distanceRev.real;
      }
      else {
        distance.real = distanceRev.real = distance.ideal;
      }
    }
  }

  return distance;
}

Distance StopDistancesManager::ComputeDistance(const StopRoute& route) {
  Distance resultDistance;
  for (size_t i = 1; i < route.size(); ++i) {
    resultDistance += ComputeDistance(StopVector{route[i - 1], route[i]});
  }
  return resultDistance;
}
