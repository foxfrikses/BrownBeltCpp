#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Email {
  string from;
  string to;
  string body;
};

istream& operator>>(istream& stream, Email& mail) {
  getline(stream, mail.from);
  getline(stream, mail.to);
  getline(stream, mail.body);
  return stream;
}

ostream& operator<<(ostream& stream, Email& mail) {
  return stream << mail.from << '\n' << mail.to << '\n' << mail.body << '\n';
}

class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const {
    if (next) {
      next->Process(std::move(email));
    }
  }

public:
  void SetNext(unique_ptr<Worker> next) {
    if (this->next) {
      this->next->SetNext(move(next));
    }
    else {
      this->next = move(next);
    }
  }
  
private:
  unique_ptr<Worker> next;
};

class Reader : public Worker {
  istream& stream;
public:
  Reader(istream& stream)
    : stream(stream)
  {}
  ~Reader() override = default;
  
  void Process(unique_ptr<Email> email) override {
    PassOn(move(email));
  }
  void Run() override {
    Email email;
    while (stream >> email) {
      Process(make_unique<Email>(email));
    }
  }
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;

public:
  Filter(function<bool(const Email&)> pred)
    : pred(move(pred))
  {}
  ~Filter() override = default;
  void Process(unique_ptr<Email> email) override {
    if (pred(*email)) {
      PassOn(move(email));
    }
  }
private:
  function<bool(const Email&)> pred;
};


class Copier : public Worker {
public:
  Copier(string to)
    : to(move(to))
  {}
  ~Copier() override = default;
  void Process(unique_ptr<Email> email) override {
    unique_ptr<Email> copy;
    if (email->to != to) {
      copy = make_unique<Email>();
      copy->from = email->from;
      copy->body = email->body;
      copy->to = to;
    }
    PassOn(move(email));
    if (copy) {
      PassOn(move(copy));
    }
  }
private:
  string to;
};

class Sender : public Worker {
  ostream& stream;
public:
  Sender(ostream& stream)
    : stream(stream)
  {}
  ~Sender() override = default;
  void Process(unique_ptr<Email> email) override {
    stream << *email;
    PassOn(move(email));
  }
};


// реализуйте класс
class PipelineBuilder {
  unique_ptr<Worker> pipeline;
public:
  explicit PipelineBuilder(istream& in)
    : pipeline(make_unique<Reader>(in))
  {}
  
  PipelineBuilder& FilterBy(Filter::Function filter) {
    pipeline->SetNext(make_unique<Filter>(filter));
    return *this;
  }
  
  PipelineBuilder& CopyTo(string recipient) {
    pipeline->SetNext(make_unique<Copier>(recipient));
    return *this;
  }
  
  PipelineBuilder& Send(ostream& out) {
    pipeline->SetNext(make_unique<Sender>(out));
    return *this;
  }
  
  unique_ptr<Worker> Build() {
    return move(pipeline);
  }
};


void TestSanity() {
  string input = (
      "erich@example.com\n"
      "richard@example.com\n"
      "Hello there\n"

      "erich@example.com\n"
      "ralph@example.com\n"
      "Are you sure you pressed the right button?\n"

      "ralph@example.com\n"
      "erich@example.com\n"
      "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;
  
  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();
  
  pipeline->Run();
  
  string expectedOutput = (
      "erich@example.com\n"
      "richard@example.com\n"
      "Hello there\n"
      
      "erich@example.com\n"
      "ralph@example.com\n"
      "Are you sure you pressed the right button?\n"
      
      "erich@example.com\n"
      "richard@example.com\n"
      "Are you sure you pressed the right button?\n"
  );
  
  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
