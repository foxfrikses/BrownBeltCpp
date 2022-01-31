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

void StopDistancesManager::SetCoords(Stop stop, Coords coords) {
  coords_[stop] = coords;
}

void StopDistancesManager::SetDistance(StopVector stopVector, double distance) {
  distances_[stopVector].real = distance;
}

Distance StopDistancesManager::ComputeDistance(StopVector stopVector) {
  auto [distanceIt, isInserted] = distances_.emplace(stopVector, Distance());

  if (isInserted || distanceIt->second.ideal == 0.0 || distanceIt->second.real == 0.0) {

    auto [distanceItRev, isInsertedRev] = distances_.emplace(StopVector {stopVector.second, stopVector.first}, 
                                                             Distance {});

    if (distanceIt->second.ideal == 0.0 || 
        distanceItRev->second.ideal == 0.0) {

      auto fstCoordsIt = coords_.find(stopVector.first);
      auto scndCoordsIt = coords_.find(stopVector.second);

      if (fstCoordsIt == coords_.end() || scndCoordsIt == coords_.end()) {
        throw runtime_error("Cannot compute distance. Stop is not found");
      }

      distanceIt->second.ideal = distanceItRev->second.ideal =
        ::DistanceBetween(fstCoordsIt->second, scndCoordsIt->second);
    }

    if (distanceIt->second.real == 0.0 || distanceItRev->second.real == 0.0) {

      if (distanceIt->second.real != 0.0) {
          distanceItRev->second.real = distanceIt->second.real;

      } else if (distanceItRev->second.real != 0.0) {
        distanceIt->second.real = distanceItRev->second.real;

      } else {
        distanceIt->second.real = distanceItRev->second.real = 
          distanceIt->second.ideal;

      }
    }
  }

  return distanceIt->second;
}

Distance StopDistancesManager::ComputeDistance(const StopRoute& route) {
  auto prevStopIt = distances_.end();

  Distance resultDistance;
  for (size_t i = 1; i < route.size(); ++i) {
    resultDistance += ComputeDistance(StopVector{route[i - 1], route[i]});
  }

  return resultDistance;
}
