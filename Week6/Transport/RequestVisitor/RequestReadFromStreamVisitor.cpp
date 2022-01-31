#include "RequestReadFromStreamVisitor.h"

#include <iostream>
#include <string>
#include <tuple>
#include <sstream>

using namespace std;

template <typename Out>
Out StreamCast(string_view sv) {
  stringstream ss;
  ss << sv;
  Out v;
  ss >> v;
  return v;
}

string_view Trunc(string_view sv) {
  
  if (auto first = sv.find_first_not_of(' ');
      first == string_view::npos) {
    sv.remove_prefix(sv.size());
  }
  else {
    sv.remove_prefix(first);

    sv.remove_suffix(sv.size() - 1 - sv.find_last_not_of(' '));
  }

  return sv;
}

struct ParseNextTokenReturnVal {
  string_view token;
  string_view delim;
};

ParseNextTokenReturnVal ParseNextTokenMultiDelim(string_view& sv, string_view delim) {
  sv = ::Trunc(sv);

  ParseNextTokenReturnVal val;

  size_t send = sv.find_first_of(delim);

  if (send == string_view::npos) {
    send = sv.size();
    val.delim = delim;
  } else {
    val.delim = sv.substr(send, 1);
  }

  val.token = ::Trunc(sv.substr(0, send));

  sv.remove_prefix(send + (send < sv.size()));

  sv = ::Trunc(sv);

  return val;
}

string_view ParseNextTokenSingleDelim(string_view& sv, string_view delim) {
  sv = ::Trunc(sv);

  size_t send = sv.find(delim);

  if (send == string_view::npos) {
    send = sv.size();
  }

  send += delim.size() - 1;

  string_view val = ::Trunc(sv.substr(0, send));

  sv.remove_prefix(send + (send < sv.size()));

  sv = ::Trunc(sv);

  return val;
}

namespace RequestVisitor
{
  ReadFromStream::ReadFromStream(std::istream &stream)
    : stream(stream)
  {}
  
  ReadFromStream::~ReadFromStream() = default;
  
  void ReadFromStream::Visit(Requests::AddBus &request) {
    string str;
    std::getline(stream, str);
    
    string_view sv(str);
    
    request.name = ParseNextTokenSingleDelim(sv, ":");
    
    std::string_view mode = "->";
    
    request.stopList.clear();
    while (!sv.empty()) {
      auto r = ::ParseNextTokenMultiDelim(sv, mode);
      request.stopList.emplace_back(r.token);
      mode = r.delim;
    }
    
    if (mode == "-") {
      request.stopList.reserve(request.stopList.size() * 2 - 1);
      for (size_t i = request.stopList.size() - 1; i > 0; --i) {
        request.stopList.push_back(request.stopList[i - 1]);
      }
    }
  }
  
  void ReadFromStream::Visit(Requests::AddStop &request) {
    string str;
    std::getline(stream, str);
    string_view sv(str);
    
    request.name = ::ParseNextTokenSingleDelim(sv, ":");
    auto latitude = ::ParseNextTokenSingleDelim(sv, ",");
    auto longitude = ::ParseNextTokenSingleDelim(sv, ",");
    
    request.latitude = ::StreamCast<double>(latitude);
    request.longitude = ::StreamCast<double>(longitude);
    
    request.distances.clear();
    while (!sv.empty()) {
      auto dist = ::ParseNextTokenSingleDelim(sv, "m");
      ::ParseNextTokenSingleDelim(sv, "to");
      string_view stopName = ::ParseNextTokenSingleDelim(sv, ",");
      double distance = ::StreamCast<double>(dist);
      
      request.distances.emplace_back(stopName, distance);
    }
  }
  
  void ReadFromStream::Visit(Requests::GetBus&request) {
    std::getline(stream, request.name);
    request.name = ::Trunc(request.name);
  }
  
  void ReadFromStream::Visit(Requests::GetStop&request) {
    std::getline(stream, request.name);
    request.name = ::Trunc(request.name);
  }
}