#include "RequestsStreamReader.h"
#include "RequestReadFromStreamVisitor.h"

#include <vector>

using namespace std;

namespace {
  vector<IRequestsReader::RequestPtr> Parse(istream &stream, IRequest::Type requestType) {
    RequestVisitor::ReadFromStream readVisitor(stream);
    
    int count;
    stream >> count;
    
    vector<IRequestsReader::RequestPtr> requests(count);
  
    string requestName;
    for (auto &request: requests) {
      stream >> requestName;
      request = IRequest::GetRequest(requestType, requestName);
      request->Access(readVisitor);
    }
    
    return requests;
  }
}

namespace RequestsReader {
  IRequestsReader::Requests
  Stream::Read(istream &stream) const {
    Requests requests;
    
    requests.base = ::Parse(stream, IRequest::Type::Base);
    requests.stat = ::Parse(stream, IRequest::Type::Stat);
    
    return requests;
  }
}