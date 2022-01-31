#pragma once

class IResponseVisitor;

class IResponse {
public:
  virtual ~IResponse() = default;
  virtual void Access(IResponseVisitor& v) = 0;
};

