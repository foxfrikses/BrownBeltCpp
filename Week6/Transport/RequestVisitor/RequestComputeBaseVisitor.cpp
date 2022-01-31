#include "RequestComputeBaseVisitor.h"

namespace RequestVisitor
{
  ComputeBase::ComputeBase(TransportDirectoryBuilder& builder)
    : builder_(builder)
  {}
  
  ComputeBase::~ComputeBase() = default;
  
  void ComputeBase::Visit(Requests::AddBus &request) {
    builder_.AddBus(std::move(request.name),
                    std::move(request.stopList));
  }
  
  void ComputeBase::Visit(Requests::AddStop &request) {
    builder_.AddStopDistanses(request.name, std::move(request.distances));
    
    builder_.AddStop(std::move(request.name),
                     request.latitude,
                     request.longitude);
  }
}