#pragma once

#include <utility>

namespace RAII
{
  template <typename Provider>
  class Booking {
  public:
    Booking(Provider* provider, int id)
      : provider(provider)
      , id(id)
    {}
    Booking(const Booking&) = delete;
    Booking(Booking&& b) {
      *this = std::move(b);
    }
  
    Booking& operator =(const Booking&) = delete;
    Booking& operator =(Booking&& b) {
      CancelOrComplete();
      provider = std::exchange(b.provider, nullptr);
      id = std::exchange(b.id,0);
      return *this;
    }
    
    ~Booking() {
      CancelOrComplete();
    }
  
  private:
    void CancelOrComplete() {
      if (provider && id != 0) {
        provider->CancelOrComplete(*this);
        id = 0;
      }
    }
    
    Provider* provider = nullptr;
    int id = 0;
  };
}
