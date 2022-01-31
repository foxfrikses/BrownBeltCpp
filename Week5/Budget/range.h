#pragma once

#include <utility>

template <typename T>
class Range final {
public:
  Range(T begin, T end)
      : begin_(std::move(begin))
      , end_(std::move(end))
  {}
  auto begin() {return begin_;}
  auto end() {return end_;}
  
  auto begin() const {return begin_;}
  auto end() const {return end_;}

private:
  T begin_, end_;
};
