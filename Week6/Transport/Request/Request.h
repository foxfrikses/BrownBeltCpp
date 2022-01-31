#pragma once

#include <string_view>
#include <memory>

class IRequestVisitor;

class IRequest {
public:
  enum class Type {Base, Stat};
  
  static std::unique_ptr<IRequest> GetRequest(Type requestType, std::string_view requestName);
  
  virtual ~IRequest() {};
  virtual void Access(IRequestVisitor& v) = 0;
};
