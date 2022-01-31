#pragma once

#include <string>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

class Database {
public:
  bool Put(const Record& record)
  {
    auto found = idToRecordData.find(record.id);
    if (found != idToRecordData.end()) {
      return false;
    }
    
    data.push_front(record);
    auto it = data.begin();
    
    idToRecordData.emplace(
      it->id,
      RecordData{
        it,
        userToRecord.emplace(it->user, it),
        timestampToRecord.emplace(it->timestamp, it),
        karmaToRecord.emplace(it->karma, it)
      }
    );
    return true;
  }
  
  const Record* GetById(const string& id) const
  {
    auto found = idToRecordData.find(id);
    if (found == idToRecordData.end()) {
      return nullptr;
    }
    return &*found->second.recordIter;
  }
  
  bool Erase(const string& id)
  {
    auto found = idToRecordData.find(id);
    if (found == idToRecordData.end()) {
      return false;
    }
  
    userToRecord.erase(found->second.userIter);
    timestampToRecord.erase(found->second.timestampIter);
    karmaToRecord.erase(found->second.karmaIter);
    data.erase(found->second.recordIter);
    idToRecordData.erase(found);
    return true;
  }
  
  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const
  {
    const auto begin = timestampToRecord.lower_bound(low);
    const auto end = timestampToRecord.upper_bound(high);
    
    auto it = begin;
    while (it != end && callback(*it->second)) {
      ++it;
    }
  }
  
  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const
  {
    const auto begin = karmaToRecord.lower_bound(low);
    const auto end = karmaToRecord.upper_bound(high);
  
    auto it = begin;
    while (it != end && callback(*it->second)) {
      ++it;
    }
  }
  
  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const
  {
    const auto [begin, end] = userToRecord.equal_range(user);
  
    auto it = begin;
    while (it != end && callback(*it->second)) {
      ++it;
    }
  }

private:
  struct RecordData;
  
  using RecordList           = list<Record>;
  using IdToRecordDataMap    = unordered_map<string, RecordData>;
  using UserToRecordMap      = unordered_multimap<string, RecordList::iterator>;
  using TimestampToRecordMap = multimap<int, RecordList::iterator>;
  using KarmaToRecordMap     = multimap<int, RecordList::iterator>;
  
  struct RecordData {
    RecordList::iterator           recordIter;
    UserToRecordMap::iterator      userIter;
    TimestampToRecordMap::iterator timestampIter;
    KarmaToRecordMap::iterator     karmaIter;
  };
  
  RecordList           data;
  IdToRecordDataMap    idToRecordData;
  UserToRecordMap      userToRecord;
  TimestampToRecordMap timestampToRecord;
  KarmaToRecordMap     karmaToRecord;
};
