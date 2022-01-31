#include "json.h"

#include <cmath>

#include <sstream>
#include <iostream>
using namespace std;

namespace Json {
  void IgnoreSpaces(istream& stream) {
    while (isspace(stream.peek())) stream.ignore(1);
  }
  

  Document::Document(Node root) : root(move(root)) {}

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;
    
    while (IgnoreSpaces(input), input.peek() != ']') {
      if (input.peek() == ',') {
        input.ignore(1);
      }
      result.push_back(LoadNode(input));
    }
    input.ignore(1);
  
    return Node(move(result));
  }

  Node LoadNumber(istream& input) {
    double d;
    input >> d;
    
    if (int(d) == d) {
      return Node(int(d));
    }
    else {
      return Node(d);
    }
  }

  Node LoadBool(istream& input) {
    bool b;
    input >> boolalpha >> b;
    return Node(b);
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    while (IgnoreSpaces(input), input.peek() != '}') {
      if (input.peek() == ',') {
        input.ignore(1);
      }
  
      Node key = LoadNode(input);
      
      IgnoreSpaces(input);
      input.ignore(1);
      
      Node value = LoadNode(input);
      
      result[key.AsString()] = move(value);
    }
    input.ignore(1);

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    IgnoreSpaces(input);
  
    char c;
    input >> c;

    Node node;
    if (c == '[') {
      node = LoadArray(input);
    } else if (c == '{') {
      node = LoadDict(input);
    } else if (c == '"') {
      node = LoadString(input);
    } else if (c == 't' || c == 'f') {
      input.putback(c);
      node = LoadBool(input);
    } else {
      input.putback(c);
      node = LoadNumber(input);
    }
    return node;
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }
  
  //////////////////////////// Upload /////////////////////////////////////////
  
  void UploadNode(ostream& output, const Node& document);
  
  void UploadArray(ostream& output, const Node& node) {
    auto& nodeArray = node.AsArray();
    
    output << '[';
    for (size_t i = 0; i < nodeArray.size(); ++i) {
      if (i != 0) output << ',';
      UploadNode(output, nodeArray[i]);
    }
    output << ']';
  }
  
  void UploadNumber(ostream& output, const Node& node) {
    if (node.IsInt()) output << node.AsInt();
    else output << node.AsDouble();
  }
  
  void UploadBool(ostream& output, const Node& node) {
    output << boolalpha << node.AsBool();
  }
  
  void UploadString(ostream& output, const Node& node) {
    output << '"' << node.AsString() << '"';
  }
  
  void UploadDict(ostream& output, const Node& node) {
    map<string, Node> result;
  
    auto& nodeMap = node.AsMap();
  
    output << '{';
    bool isFirst = true;
    for (auto& [key, value] : nodeMap) {
      if (isFirst) isFirst = false;
      else output << ',';
  
      UploadString(output, key);
      output << ':';
      UploadNode(output, value);
    }
    output << '}';
  }
  
  void UploadNode(ostream& output, const Node& node) {
    if (node.IsInt() || node.IsDouble()) {
      UploadNumber(output, node);
    }
    else if (node.IsBool()) {
      UploadBool(output, node);
    }
    else if (node.IsString()) {
      UploadString(output, node);
    }
    else if (node.IsArray()) {
      UploadArray(output, node);
    }
    else if (node.IsMap()) {
      UploadDict(output, node);
    }
    else {throw runtime_error("Not defined UploadNode for the node type");}
  }
  
  void Upload(ostream& output, const Node& node) {
    UploadNode(output, node);
  }
}
