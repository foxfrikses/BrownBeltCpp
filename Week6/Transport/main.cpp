#include "Request.h"
#include "JsonRequestsReader.h"
#include "ComputeStatRequestVisitor.h"
#include "ComputeBaseRequestVisitor.h"

#include "Response.h"
#include "JsonResponseWriter.h"

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
  auto requests = RequestsReader::Json().Read(cin);
  
  TransportDirectoryBuilder builder;
  
  ComputeBaseRequests(builder, move(requests.base));
  
  builder.SetRoutingSettings(move(requests.routingSettings));

  TransportDirectory directory = builder.Build();

  auto responses = ::ComputeStatRequests(directory, move(requests.stat));
  
  ResponseWriter::Json().Write(cout, move(responses));
  
  return 0;
}

