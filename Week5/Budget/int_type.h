#pragma once

#define INT_TYPE(TYPE_NAME)                        \
  class TYPE_NAME final {                          \
    int data_;                                     \
  public:                                          \
    explicit constexpr TYPE_NAME(int value)        \
      : data_(value)                               \
    {}                                             \
    constexpr operator int() const {return data_;} \
  }
