#include "RequestsJsonReader.h"
#include "RequestReadFromJsonVisitor.h"

#include "json.h"

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;
using namespace Json;



namespace {
  vector<IRequestsReader::RequestPtr> Parse(const Node &node, IRequest::Type requestType) {
    auto &nodes = node.AsArray();
    
    vector<IRequestsReader::RequestPtr> requests;
    requests.reserve(nodes.size());
    
    for (auto &node: nodes) {
      auto& requestName = node.AsMap().at("type").AsString();
      auto request= IRequest::GetRequest(requestType, requestName);
      RequestVisitor::ReadFromJson visitor(node);
      request->Access(visitor);
      requests.push_back(move(request));
    }
    
    return requests;
  }
}

namespace RequestsReader {
  IRequestsReader::Requests
  Json::Read(istream &stream) const {
    auto document = Load(stream);
    
    auto &base = document.GetRoot().AsMap().at("base_requests");
    auto &stat = document.GetRoot().AsMap().at("stat_requests");
    
    Requests requests;
    
    requests.base = ::Parse(base, IRequest::Type::Base);
    requests.stat = ::Parse(stat, IRequest::Type::Stat);
    
    return requests;
  }
}