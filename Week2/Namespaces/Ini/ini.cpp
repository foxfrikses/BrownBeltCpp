#include "ini.h"

#include <sstream>
#include <string_view>

using namespace std;

namespace Ini {
  Section& Document::AddSection(
    std::string name
  )
  {
    return sections[std::move(name)];
  }
  
  const Section& Document::GetSection(
    const std::string& name
  ) const
  {
    return sections.at(name);
  }
  
  size_t Document::SectionCount() const {
    return sections.size();
  }
  
  Document Load(istream& input) {
    string str;
    Document doc;
  
    Section* sec = nullptr;
    
    while (getline(input, str)) {
      if (str.empty()) continue;
  
      if (str.front() == '[') {
        sec = &doc.AddSection(str.substr(1, str.size() - 2));
        continue;
      }
  
      auto eqCh = str.find_first_of('=');
      sec->emplace(str.substr(0, eqCh),
                   str.substr(eqCh + 1, str.size() - (eqCh + 1)));
    }
    
    return doc;
  }
}
