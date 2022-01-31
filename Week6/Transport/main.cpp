#include "Request.h"
#include "RequestsStreamReader.h"
#include "RequestsJsonReader.h"
#include "RequestComputeStatVisitor.h"
#include "RequestComputeBaseVisitor.h"

#include "Response.h"
#include "ResponsesStreamWriter.h"
#include "ResponsesJsonWriter.h"

#include "TransportDirectory.h"
#include "TransportDirectoryBuilder.h"

#include <iostream>
#include <vector>

using namespace std;

void ComputeBaseRequests(TransportDirectoryBuilder &builder,
                         vector<unique_ptr<IRequest>> requests) {
  RequestVisitor::ComputeBase computer(builder);
  
  for (auto &r : requests) {
    r->Access(computer);
  }
}

vector<unique_ptr<IResponse>> ComputeStatRequests(const TransportDirectory &dir,
                                                  vector<unique_ptr<IRequest>> requests) {
  RequestVisitor::ComputeStat computer(dir);
  
  for (auto &r : requests) {
    r->Access(computer);
  }
  
  return computer.ReleaseResponses();
}

int main() {
  //auto requests = RequestsReader::Stream().Read(cin);
  auto requests = RequestsReader::Json().Read(cin);
  
  TransportDirectoryBuilder builder;
  
  ComputeBaseRequests(builder, move(requests.base));

  TransportDirectory directory = builder.GetTransportDirectory();

  auto responses = ::ComputeStatRequests(directory, move(requests.stat));
  
  //ResponseWriter::Stream().Write(cout, move(responses));
  ResponseWriter::Json().Write(cout, move(responses));
  
  return 0;
}

