#include "ResponseWriteToStreamVisitor.h"

using namespace std;

namespace ResponseVisitor {
  WriteToStream::WriteToStream(std::ostream &stream)
      : stream(stream) {
    stream.precision(6);
  }
  
  void WriteToStream::Visit(Responses::GetBus &response) {
    stream << "Bus " << response.busName << ": ";
    
    if (!response.data) {
      stream << "not found";
    } else {
      stream << response.data->stopsOnRoute << " stops on route, " <<
             response.data->uniqueStops << " unique stops, " <<
             response.data->routeLength << " route length, " <<
             response.data->curvature << " curvature";
    }
    
    stream << '\n';
  }
  
  void WriteToStream::Visit(Responses::GetStop &response) {
    stream << "Stop " << response.stopName << ": ";
    
    if (!response.data) {
      stream << "not found";
    } else if (response.data->buses.empty()) {
      stream << "no buses";
    } else {
      stream << "buses";
      for (auto &bus: response.data->buses) {
        stream << ' ' << bus;
      }
    }
    
    stream << '\n';
  }
}