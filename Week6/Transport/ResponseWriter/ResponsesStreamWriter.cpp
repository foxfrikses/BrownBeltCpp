#include "ResponsesStreamWriter.h"

#include "ResponseWriteToStreamVisitor.h"

using namespace std;

namespace ResponseWriter {
  void Stream::Write(ostream& stream, Responses responses) const {
    ResponseVisitor::WriteToStream writeVisitor(stream);
    
    for (auto& response : responses) {
      response->Access(writeVisitor);
    }
  }
}
