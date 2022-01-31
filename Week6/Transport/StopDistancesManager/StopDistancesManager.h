#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

struct Coords {
  double latitude = 0.0;
  double longitude = 0.0;
};

struct Distance {
  double ideal = 0.0, real = 0.0;
  
  Distance& operator+=(const Distance &other);
};

struct PairHash {
  template <typename T, typename U>
  std::size_t operator() (const std::pair<T, U> &v) const {
    return std::hash<T>()(v.first) + (1'000'000'007)  * std::hash<U>()(v.second);
  }
};

class StopDistancesManager {
public:
  using Stop = std::string_view;
  using StopRoute = std::vector<Stop>;
  using StopVector = std::pair<Stop, Stop>;

  void SetCoords(Stop stop, Coords coords);
  void SetDistance(StopVector stopVector, double distance);
  Distance ComputeDistance(const StopRoute& route);

private:
  Distance ComputeDistance(StopVector stopVector);

  std::unordered_map<Stop, Coords> coords_;
  std::unordered_map<StopVector, Distance, PairHash> distances_;
};
