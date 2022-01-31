#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

bool IsSubdomain(string_view subdomain, string_view domain) {
  if (subdomain.size() < domain.size()) {
    return false;
  }
  
  return subdomain.substr(0, domain.size()) == domain;
}

vector<string> ReadDomains(istream& stream) {
  size_t count;
  
  {
    string count_str;
    getline(stream, count_str);
    stringstream count_ss(count_str);
    count_ss >> count;
  }
  
  vector<string> domains(count);
  for (auto& domain : domains) {
    getline(stream, domain);
    reverse(domain.begin(), domain.end());
  }
  return domains;
}

vector<string> SortAndRemoveSubdomains(vector<string> domains) {
  sort(domains.begin(), domains.end());
  
  if (!domains.empty()) {
    list<string> list_domains(make_move_iterator(domains.begin()),make_move_iterator(domains.end()));
    
    auto it = list_domains.begin();
    auto it_next = next(it);
    
    while (it_next != list_domains.end()) {
      if (IsSubdomain(*it_next, *it)) {
        list_domains.erase(it_next);
      }
      else {
        it = move(it_next);
      }
  
      it_next = next(it);
    }
    
    domains.resize(list_domains.size());
    move(make_move_iterator(list_domains.begin()),
         make_move_iterator(list_domains.end()),
         domains.begin());
  }
  
  return domains;
}

int main() {
  const vector<string> banned_domains = SortAndRemoveSubdomains(ReadDomains(cin));
  const vector<string> domains_to_check = ReadDomains(cin);
  
  for (const string& domain : domains_to_check) {
    const auto it = upper_bound(banned_domains.begin(), banned_domains.end(), domain);
    if (it == banned_domains.begin() || !IsSubdomain(domain, *prev(it))) {
      cout << "Good" << endl;
    } else {
      cout << "Bad" << endl;
    }
  }
  return 0;
}
