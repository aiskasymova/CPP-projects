#pragma once
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
class String {
 private:
  char* str_;
  size_t size_;
  size_t capacity_;

 public:
  String();
  String(size_t size, char character);
  explicit String(size_t cap);
  String(const char* new_str);
  String(const String& obj);
  String& operator=(const String& obj);
  ~String();
  void Clear();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  void Swap(String& other);
  char& operator[](size_t index);
  const char& operator[](size_t index) const;
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;
  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;
  char* Data();
  const char* Data() const;
  friend String operator+(const String& lhs, const String& rhs);
  String& operator+=(const String& other);
  String& operator+=(char character);
  String& operator*=(int number);
  std::vector<String> Split(const String& delim = " ") const;
  String Join(const std::vector<String>& strings) const;
  friend bool operator<(const String& lhs, const String& rhs);
  friend std::istream& operator>>(std::istream& input, String& str);
  friend std::ostream& operator<<(std::ostream& out, const String& str);
  friend size_t Find(const String& str, const String& delim, size_t start);
};
bool operator>(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
String operator*(const String& str, int number);
