#include "ResponsesJsonWriter.h"

#include "ResponseWriteToJsonVisitor.h"

using namespace std;
using namespace Json;

namespace ResponseWriter {
  void Json::Write(ostream& stream, Responses responses) const {
    std::vector<Node> nodes;
    
    ResponseVisitor::WriteToJson writeVisitor(nodes);
    
    for (auto& response : responses) {
      response->Access(writeVisitor);
    }
  
    Node node(move(nodes));
    Upload(stream, node);
  }
}
