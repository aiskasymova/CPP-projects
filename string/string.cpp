#include "string.hpp"

String::String() : str_(nullptr), size_(0), capacity_(0) {}
String::String(size_t new_size, char character) {
  capacity_ = new_size * 2;
  str_ = new char[capacity_];
  size_ = new_size;
  std::fill(str_, str_ + size_, character);
  str_[size_] = '\0';
}
String::String(size_t cap) {
  capacity_ = cap;
  str_ = new char[capacity_ + 1];
  size_ = cap;
}
String::String(const char* new_str) {
  size_ = std::strlen(new_str);
  capacity_ = size_ * 2;
  str_ = new char[capacity_ + 1];
  strcpy(str_, new_str);
  str_[size_] = '\0';
}
String::String(const String& obj) {
  str_ = new char[obj.capacity_ + 1];
  size_ = obj.size_;
  if (obj.str_ != nullptr) {
    strcpy(str_, obj.str_);
  }
  capacity_ = obj.capacity_;
  str_[size_] = '\0';
}
String& String::operator=(const String& obj) {
  if (&obj != this) {
    delete[] str_;
    size_ = obj.size_;
    str_ = new char[size_ + 1];
    if (obj.str_ != nullptr) {
      strcpy(str_, obj.str_);
    }
    capacity_ = obj.capacity_;
    str_[size_] = '\0';
  }
  return *this;
}
String::~String() { delete[] str_; }
void String::Clear() { size_ = 0; }
void String::PushBack(char character) {
  if (size_ + 1 > capacity_) {
    Resize(size_ + 1);
    str_[size_ - 1] = character;
    str_[size_] = '\0';
  } else {
    str_[size_++] = character;
    str_[size_] = '\0';
  }
}
void String::PopBack() {
  if (str_ != nullptr) {
    str_[size_ - 1] = '\0';
    --size_;
  }
}
void String::Resize(size_t new_size) {
  if (new_size <= capacity_) {
    size_ = new_size;
    capacity_ = new_size * 2;
    return;
  }
  capacity_ = new_size * 2;
  char* tmp = new char[capacity_ + 1];
  if (str_ != nullptr) {
    strcpy(tmp, str_);
  }
  delete[] str_;
  tmp[size_] = '\0';
  size_ = new_size;
  str_ = tmp;
}
void String::Resize(size_t new_size, char character) {
  size_t size_copy = size_;
  Resize(new_size);
  if (new_size > size_copy) {
    for (size_t i = size_copy; i < new_size; ++i) {
      str_[i] = character;
    }
  }
}
void String::Reserve(size_t new_cap) {
  capacity_ = capacity_ > new_cap ? capacity_ : new_cap;
  char* tmp = new char[capacity_ + 1];
  if (str_ != nullptr) {
    strcpy(tmp, str_);
  }
  delete[] str_;
  tmp[size_] = '\0';
  str_ = tmp;
}
void String::ShrinkToFit() {
  if (capacity_ > size_) {
    capacity_ = size_;
  }
}
void String::Swap(String& other) {
  char* temp = str_;
  str_ = other.str_;
  other.str_ = temp;
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}
char& String::operator[](size_t index) { return str_[index]; }
const char& String::operator[](size_t index) const { return str_[index]; }
char& String::Front() { return str_[0]; }
const char& String::Front() const { return str_[0]; }
char& String::Back() { return str_[size_ - 1]; }
const char& String::Back() const { return str_[size_ - 1]; }
bool String::Empty() const { return size_ == 0; }
size_t String::Size() const { return size_; }
size_t String::Capacity() const { return capacity_; }
char* String::Data() { return str_; }
const char* String::Data() const { return str_; }
bool operator<(const String& lhs, const String& rhs) {
  size_t min_size;
  min_size = (lhs.size_ < rhs.size_) ? lhs.size_ : rhs.size_;
  for (size_t i = 0; i < min_size; ++i) {
    if (lhs.str_[i] < rhs.str_[i]) {
      return true;
    }
  }
  return ((lhs.size_ - min_size) < (rhs.size_ - min_size));
}
bool operator>(const String& lhs, const String& rhs) { return rhs < lhs; }
bool operator<=(const String& lhs, const String& rhs) { return !(lhs > rhs); }
bool operator>=(const String& lhs, const String& rhs) { return !(lhs < rhs); }
bool operator==(const String& lhs, const String& rhs) {
  return !(lhs < rhs || rhs < lhs);
}
bool operator!=(const String& lhs, const String& rhs) { return !(lhs == rhs); }
std::ostream& operator<<(std::ostream& out, const String& str) {
  out << str.str_;
  return out;
}
std::istream& operator>>(std::istream& input, String& str) {
  const size_t kAmount = 100;
  static char buffer[kAmount];
  input.getline(buffer, kAmount);
  size_t size = strlen(buffer);
  if (size > str.capacity_) {
    delete[] str.str_;
    str.str_ = new char[size + 1];
    str.capacity_ = size;
  }
  strcpy(str.str_, buffer);
  str.size_ = size;
  return input;
}
String& String::operator+=(const String& other) {
  if (size_ + other.size_ < capacity_) {
    strcpy(str_ + size_, other.str_);
    size_ += other.size_;
    return *this;
  }
  size_t old_size = size_;
  Reserve(size_ + other.size_ + 1);
  size_ += other.size_;
  strcpy(str_ + old_size, other.str_);
  return *this;
}
String& String::operator+=(char character) {
  if (size_ + 1 <= capacity_) {
    str_[size_] = character;
    str_[size_ + 1] = '\0';
    ++size_;
    return *this;
  }
  size_t old_size = size_;
  Resize(size_ + 1);
  str_[old_size] = character;
  str_[size_] = '\0';
  return *this;
}
String operator+(const String& lhs, const String& rhs) {
  String sum = lhs;
  sum += rhs;
  return sum;
}
String operator*(const String& str, int number) {
  size_t new_cap = str.Size() * number;
  String res(new_cap);
  for (size_t i = 0; i < new_cap; ++i) {
    res[i] = str[i % str.Size()];
  }
  res[new_cap] = '\0';
  return res;
}
String& String::operator*=(int number) {
  *this = *this * number;
  return *this;
}
size_t Find(const String& str, const String& delim, size_t start) {
  for (size_t i = start; i < str.size_; ++i) {
    if (str.str_[i] == delim.str_[0] && delim.size_ == 1) {
      return i;
    }
    if (str.str_[i] == delim.str_[0] && delim.size_ > 1) {
      if (str.str_[i + 1] == delim.str_[1]) {
        return i;
      }
    }
  }
  return -1;
}

String Substr(const String& str, size_t start, size_t end) {
  String substr;
  size_t ind = start;
  while (ind < end) {
    substr += str[ind];
    ++ind;
  }
  return substr;
}
std::vector<String> String::Split(const String& delim) const {
  std::vector<String> strings;
  if (size_ < delim.size_) {
    strings.emplace_back("");
    return strings;
  }
  if (size_ == 0 && delim.size_ == 0) {
    strings.emplace_back("");
    strings.emplace_back("");
    return strings;
  }
  size_t start = 0;
  size_t end;
  String word;
  while ((Find(*this, delim, start)) != SIZE_MAX) {
    end = Find(*this, delim, start);
    word = Substr(*this, start, end);
    start = end + delim.size_;
    strings.push_back(word);
  }
  strings.push_back(Substr(*this, start, size_));
  return strings;
}
String String::Join(const std::vector<String>& strings) const {
  if (strings.empty() || strings[0] == "") {
    return "";
  }
  if (*this == "") {
    return strings[0];
  }
  String joined_str;
  for (size_t i = 0; i <= strings.size(); ++i) {
    if (i % 2 == 0) {
      joined_str += strings[i % strings.size()];
    } else {
      joined_str += (*this);
    }
  }
  return joined_str;
}
