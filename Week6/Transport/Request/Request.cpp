#include "Request.h"

#include "AddBusRequest.h"
#include "AddStopRequest.h"
#include "GetBusRequest.h"
#include "GetStopRequest.h"
#include "RouteRequest.h"

#include <unordered_map>
#include <memory>

using namespace std;

using RequestPtr = std::unique_ptr<IRequest>;
typedef std::unique_ptr<IRequest>(*CreateRequestFunc)();

namespace Base {
  RequestPtr CreateBusRequest() {return std::make_unique<Requests::AddBus>();}
  RequestPtr CreateStopRequest() {return std::make_unique<Requests::AddStop>();}
  
  unordered_map<string_view, CreateRequestFunc> RequestCreators {
      {string_view("Bus"), CreateBusRequest},
      {string_view("Stop"), CreateStopRequest}
  };
}

namespace Stat {
  RequestPtr CreateBusRequest() {return std::make_unique<Requests::GetBus>();}
  RequestPtr CreateStopRequest() {return std::make_unique<Requests::GetStop>();}
  RequestPtr CreateRouteRequest() {return std::make_unique<Requests::Route>();}
  
  unordered_map<string_view, CreateRequestFunc> RequestCreators {
      {string_view("Bus"), CreateBusRequest},
      {string_view("Stop"), CreateStopRequest},
      {string_view("Route"), CreateRouteRequest}
  };
}

RequestPtr IRequest::GetRequest(
  Type requestType,
  std::string_view requestName
)
{
  std::optional<CreateRequestFunc> func;
  
  switch (requestType) 	{
    case Type::Stat:
      if (auto found = Stat::RequestCreators.find(requestName);
          found != Stat::RequestCreators.end()) {
        func = found->second;
      }
      break;

    case Type::Base:
      if (auto found = Base::RequestCreators.find(requestName);
          found != Base::RequestCreators.end()) {
        func = found->second;
      }
      break;

    default:
      break;
  }
  
  RequestPtr ptr;
  if (func) {
    ptr = func.value()();
  }
  return ptr;
}
