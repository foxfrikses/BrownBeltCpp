#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <numeric>
#include <optional>

using namespace std;

struct Person {
  string name;
  int age, income;
  bool is_male;
};

vector<Person> ReadPeople(istream& input) {
  int count;
  input >> count;
  
  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }
  
  return result;
}

int main() {
  const vector<Person> people = ReadPeople(cin);
  
  const vector<int> wealthiestPeopleTotalIncome = [](const vector<Person>& people) {
    vector<int> income;
    income.reserve(people.size());
    for (auto& person : people) {
      income.push_back(person.income);
    }
    sort(rbegin(income), rend(income));
    for (size_t i = 1; i < income.size(); ++i) {
      income[i] += income[i - 1];
    }
    return income;
  }(people);
  
  const map<int, int> maturityAgeToAdultCount =  [] (const vector<Person>& people) {
    unordered_map<int, int> ageToCountUMap;
    for (auto& person : people) {
      ++ageToCountUMap[person.age];
    }
    map<int, int> ageToCountMap(begin(ageToCountUMap),
                                end(ageToCountUMap));
    {
      auto it = ageToCountMap.rbegin();
      auto nextIt = next(it);
      while (nextIt != ageToCountMap.rend()) {
        nextIt->second += it->second;
        it = nextIt;
        ++nextIt;
      }
    }
    return ageToCountMap;
  }(people);
  
  const auto [mostPopularMaleName, mostPopularFemaleName] = [](const vector<Person>& people) {
    unordered_map<string_view, int> maleNameCount;
    unordered_map<string_view, int> femaleNameCount;
    for (auto& person : people) {
      if (person.is_male) {++maleNameCount[person.name];}
      else                {++femaleNameCount[person.name];}
    }
    map<string_view, int> maleNameCountM(begin(maleNameCount), end(maleNameCount));
    map<string_view, int> femaleNameCountM(begin(femaleNameCount), end(femaleNameCount));
    
    auto mostPopular = [](const map<string_view, int>& names) -> optional<string> {
      if (names.empty()) {return nullopt;}
      
      string_view mostPopularName;
      int maxNameCount = 0;
      for (auto& [name, count] : names) {
        if (maxNameCount < count) {
          maxNameCount = count;
          mostPopularName = name;
        }
      }
      return string(mostPopularName);
    };
    
    return pair<optional<string>, optional<string>>(
      mostPopular(maleNameCountM),
      mostPopular(femaleNameCountM)
    );
  }(people);
  
  for (string command; cin >> command; ) {
    if (command == "AGE") {
      int adult_age;
      cin >> adult_age;
      
      int adult_count = 0;
      if (auto it = maturityAgeToAdultCount.upper_bound(adult_age - 1);
          it != maturityAgeToAdultCount.end()) {
        adult_count = it->second;
      }
      
      cout << "There are " << adult_count << " adult people for maturity age " << adult_age << endl;
    }
    else if (command == "WEALTHY") {
      int count;
      cin >> count;
      count = std::min(int(wealthiestPeopleTotalIncome.size()), count);
      
      cout << "Top-" << count << " people have total income " << wealthiestPeopleTotalIncome[count - 1] << endl;
    } else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;
      
      const optional<string>& mostPopularName = ((gender == 'M')
          ? mostPopularMaleName
          : mostPopularFemaleName);
      
      if (!mostPopularName.has_value()) {
        cout << "No people of gender " << gender << endl;
      } else {
        cout << "Most popular name among people of gender " << gender << " is "
             << mostPopularName.value() << endl;
      }
    }
  }
}
