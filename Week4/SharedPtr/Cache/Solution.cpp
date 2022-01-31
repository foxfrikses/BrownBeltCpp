#include "Common.h"

#include <unordered_map>
#include <map>
#include <list>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  )
    : maxMemory_(settings.max_memory)
    , unpacker(move(books_unpacker))
  {}
  
  BookPtr GetBook(const string& book_name) override {
    lock_guard lock(mutex_);
    
    if (auto found = namedBooks_.find(book_name);
        found != namedBooks_.end()) {
      
      auto& book = found->second;
      if (book->rangedBooksIt != prev(rangedBooks_.end())) {
        auto node = rangedBooks_.extract(book->rangedBooksIt);
        node.key() = prev(rangedBooks_.end())->first + 1;
        book->rangedBooksIt = rangedBooks_.insert(std::move(node)).position;
      }
      return book->book;
    }
    
    auto unpackedBook = unpacker->UnpackBook(book_name);
    if (!unpackedBook) {
      return move(unpackedBook);
    }
    
    if (unpackedBook->GetContent().size() > maxMemory_) {
      books_.clear();
      rangedBooks_.clear();
      namedBooks_.clear();
      return move(unpackedBook);
    }
    
    currentMemory_ += unpackedBook->GetContent().size();
    Shrink();
    
    auto& book = books_.emplace_back();
    book.book = move(unpackedBook);
    book.rangedBooksIt = rangedBooks_.emplace_hint(
        rangedBooks_.end(),
        rangedBooks_.empty() ? 0 : prev(rangedBooks_.end())->first + 1,
        prev(books_.end()));
    book.namedBooksIt = namedBooks_.emplace(book_name, prev(books_.end())).first;
    
    return book.book;
  }

private:
  void Shrink() {
    while (currentMemory_ > maxMemory_ && !rangedBooks_.empty()) {
      auto bookIt = rangedBooks_.begin()->second;
      
      currentMemory_ -= bookIt->book->GetContent().size();
      
      namedBooks_.erase(bookIt->namedBooksIt);
      rangedBooks_.erase(bookIt->rangedBooksIt);
      books_.erase(bookIt);
    }
  }

private:
  shared_ptr<IBooksUnpacker> unpacker;
  
  size_t currentMemory_ = 0;
  size_t maxMemory_;
  
  struct Book;
  
  using Rang = size_t;
  using Books = list<Book>;
  using RangedBooks = map<Rang, Books::iterator>;
  using NamedBooks = unordered_map<string, Books::iterator>;
  
  struct Book {
    BookPtr book;
    RangedBooks::iterator rangedBooksIt;
    NamedBooks::iterator namedBooksIt;
  };
  
  list<Book> books_;
  NamedBooks namedBooks_;
  RangedBooks rangedBooks_;
  mutex mutex_;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
)
{
  return make_unique<LruCache>(move(books_unpacker), settings);
}
