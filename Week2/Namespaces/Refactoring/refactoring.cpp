#include "xml.h"
#include "json.h"

#include "test_runner.h"

#include <vector>
#include <string>
#include <map>

Json::Document XmlToJson(const Xml::Document& doc) {
  using namespace std;
  using namespace Json;
  
  vector<Node> result;
  
  for (const auto& xmlNode : doc.GetRoot().Children()) {
    result.emplace_back(map<string, Node>{
        {"category", Node{xmlNode.AttributeValue<string>("category")}},
        {"amount", Node(xmlNode.AttributeValue<int>("amount"))}
    });
  }
  
  return Document(Node(std::move(result)));
}

Xml::Document JsonToXml(const Json::Document& doc, std::string root_name) {
  Xml::Node root(move(root_name), {});
  
  for (const auto& jsonNode : doc.GetRoot().AsArray()) {
    root.AddChild(Xml::Node("spend", {
        {"category", jsonNode.AsMap().at("category").AsString()},
        {"amount", std::to_string(jsonNode.AsMap().at("amount").AsInt())},
    }));
  }
  
  return Xml::Document(std::move(root));
}

void TestXmlToJson() {
  using std::vector;
  using std::string;
  
  Xml::Node root("july", {});
  root.AddChild({"spend", {{"category", "travel"}, {"amount", "23400"}}});
  root.AddChild({"spend", {{"category", "food"}, {"amount", "5000"}}});
  root.AddChild({"spend", {{"category", "transport"}, {"amount", "1150"}}});
  root.AddChild({"spend", {{"category", "sport"}, {"amount", "12000"}}});
  const Xml::Document xml_doc(std::move(root));
  
  const auto json_doc = XmlToJson(xml_doc);
  
  const auto& json_items = json_doc.GetRoot().AsArray();
  ASSERT_EQUAL(json_items.size(), 4u);
  
  const vector<string> expected_category = {"travel", "food", "transport", "sport"};
  const vector<int> expected_amount = {23400, 5000, 1150, 12000};
  
  for (size_t i = 0; i < json_items.size(); ++i) {
    const string feedback_msg = "i = " + std::to_string(i);
    
    AssertEqual(json_items[i].AsMap().at("category").AsString(),
                expected_category[i], feedback_msg);
    AssertEqual(json_items[i].AsMap().at("amount").AsInt(),
                expected_amount[i], feedback_msg);
  }
}

void TestJsonToXml() {
  using namespace std;
  using Json::Node;
  
  const Json::Document json_doc{Node(vector<Node>{
      Node(map<string, Node>{
          {"category", Node("food")}, {"amount", Node(2500)}
      }),
      Node(map<string, Node>{
          {"category", Node("transport")}, {"amount", Node(1150)}
      }),
      Node(map<string, Node>{
          {"category", Node("restaurants")}, {"amount", Node(5780)}
      }),
      Node(map<string, Node>{
          {"category", Node("clothes")}, {"amount", Node(7500)}
      }),
      Node(map<string, Node>{
          {"category", Node("travel")}, {"amount", Node(23740)}
      }),
      Node(map<string, Node>{
          {"category", Node("sport")}, {"amount", Node(12000)}
      }),
  })};
  
  const string root_name = "month";
  const auto xml_doc = JsonToXml(json_doc, root_name);
  const Xml::Node& root = xml_doc.GetRoot();
  
  ASSERT_EQUAL(root.Name(), root_name);
  const auto& children = root.Children();
  ASSERT_EQUAL(children.size(), 6u);
  
  const vector<string> expected_category = {
      "food", "transport", "restaurants", "clothes", "travel", "sport"
  };
  const vector<int> expected_amount = {2500, 1150, 5780, 7500, 23740, 12000};
  
  for (size_t i = 0; i < children.size(); ++i) {
    const string feedback_msg = "i = " + std::to_string(i);
    AssertEqual(children[i].Name(), "spend", feedback_msg);
    AssertEqual(children[i].AttributeValue<string>("category"),expected_category[i], feedback_msg);
    AssertEqual(children[i].AttributeValue<int>("amount"), expected_amount[i], feedback_msg);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestXmlToJson);
  RUN_TEST(tr, TestJsonToXml);
  return 0;
}
