#include <forward_list>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
  using BucketList = std::forward_list<Type>;
  
  std::vector<BucketList> buckets;
  const Hasher hasher;

public:
  HashSet(size_t num_buckets, const Hasher& hasher = {})
      : buckets(num_buckets)
      , hasher(hasher)
  {}
  
  void Add(const Type& value)
  {
    auto& bucket = _GetBucket(value);
    auto beforeIt = _GetBeforeIt(bucket, value);
    if (std::next(beforeIt) == bucket.end()) {
      bucket.push_front(value);
    }
  }
  
  bool Has(const Type& value) const
  {
    auto& bucket = GetBucket(value);
    auto found = std::find(bucket.begin(), bucket.end(), value);
    return bucket.end() != found;
  }
  
  void Erase(const Type& value)
  {
    auto& bucket = _GetBucket(value);
    auto beforeIt = _GetBeforeIt(bucket, value);
    bucket.erase_after(beforeIt);
  }
  
  const BucketList& GetBucket(const Type& value) const
  {
    return buckets[hasher(value) % buckets.size()];
  }

private:
  BucketList& _GetBucket(const Type& value)
  {
    return buckets[hasher(value) % buckets.size()];
  }
  
  static auto _GetBeforeIt(BucketList& bucket, const Type& value) {
    auto beforeIt = bucket.before_begin();
    auto it = bucket.begin();
    while (it != bucket.end() && *it != value) {
      beforeIt = it;
      ++it;
    }
    return beforeIt;
  }
};