#pragma once

#include <istream>
#include <ostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

  class Node : std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            int,
                            double,
                            bool,
                            std::string> {
  public:
    using variant::variant;

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }
    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }
    int AsInt() const {
        return std::get<int>(*this);
    }
    double AsDouble() const {
      if (std::holds_alternative<double>(*this))
        return std::get<double>(*this);
      else
        return std::get<int>(*this);
    }
    bool AsBool() const {
      return std::get<bool>(*this);
    }
    const auto& AsString() const {
      return std::get<std::string>(*this);
    }
  
    bool IsArray() const {
      return std::holds_alternative<std::vector<Node>>(*this);
    }
    bool IsMap() const {
      return std::holds_alternative<std::map<std::string, Node>>(*this);
    }
    bool IsInt() const {
      return std::holds_alternative<int>(*this);
    }
    double IsDouble() const {
      return std::holds_alternative<double>(*this) || std::holds_alternative<int>(*this);
    }
    bool IsBool() const {
      return std::holds_alternative<bool>(*this);
    }
    bool IsString() const {
      return std::holds_alternative<std::string>(*this);
    }
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);
  
  void Upload(std::ostream& input, const Node& document);
}

