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

class StopDistancesManager {
public:
  using Stop = std::string_view;
  using StopRoute = std::vector<Stop>;
  struct StopVector {Stop src, dst;};

  void SetCoords(Stop stop, Coords coords);
  void SetDistance(StopVector stopVector, double distance);

  Distance ComputeDistance(const StopRoute& route);
  Distance ComputeDistance(StopVector stopVector);

  StopDistancesManager() = default;
  
  StopDistancesManager(const StopDistancesManager& ) = delete;
  StopDistancesManager& operator=(const StopDistancesManager& ) = delete;

  StopDistancesManager(StopDistancesManager&&);
  StopDistancesManager& operator=(StopDistancesManager&&);

private:
  struct StopVectorHash {
    std::hash<Stop> hash;
    std::size_t operator() (const StopVector& v) const {
      return hash(v.src) + (1'000'000'007)  * hash(v.dst);
    }
  };
  struct StopVectorEqual {
    bool operator() (const StopVector& lhs, const StopVector& rhs) const {
      return lhs.dst == rhs.dst && lhs.src == rhs.src;
    }
  };

  std::unordered_map<Stop, Coords> coords_;
  std::unordered_map<StopVector, Distance, StopVectorHash, StopVectorEqual> distances_;
};
