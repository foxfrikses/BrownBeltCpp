#include "date.h"

std::istream& operator>>(std::istream& stream, Date& rhs) {
  int day, month, year;
  if (!(stream >> year) ||
      stream.peek() != '-' ||
      !stream.ignore(1) ||
      !std::isdigit(stream.peek()) ||
      !(stream >> month) ||
      stream.peek() != '-' ||
      !stream.ignore(1) ||
      !std::isdigit(stream.peek()) ||
      !(stream >> day)) {
    throw std::runtime_error("Incorrect Date format");
  }
  rhs = Date(Day(day), Month(month), Year(year));
  return stream;
}
