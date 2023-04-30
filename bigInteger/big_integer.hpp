#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
class BigInt {
 private:
  std::vector<int> digits_;
  int sign_;

 public:
  BigInt();
  BigInt(int64_t big_num);
  explicit BigInt(std::string str);
  BigInt(const BigInt& obj);
  BigInt& operator=(const BigInt& obj);
  BigInt& Plus(const BigInt& rhs);
  BigInt& Minus(const BigInt& rhs);
  BigInt& MinusLess(const BigInt& rhs, int size);
  BigInt& MinusGreater(const BigInt& rhs, int size);
  friend BigInt MyMinus(const BigInt& lhs, const BigInt& rhs);
  void Update(size_t pos, bool flag);
  void Update(const BigInt& other, size_t pos, int extra, bool flag);
  BigInt& operator+=(const BigInt& other);
  BigInt& operator-=(const BigInt& other);
  void Helper(const std::vector<int>& vec, int val);
  BigInt& operator*=(const BigInt& other);
  BigInt& operator/=(const BigInt& other);
  BigInt& operator%=(const BigInt& other);
  BigInt& operator++();
  BigInt operator++(int);
  BigInt& operator--();
  BigInt operator--(int);
  BigInt& operator-();
  friend bool operator==(const BigInt& lhs, const BigInt& rhs);
  friend bool operator<(const BigInt& lhs, const BigInt& rhs);
  friend std::istream& operator>>(std::istream& stream, BigInt& big_int);
  friend std::ostream& operator<<(std::ostream& out, const BigInt& big_int);
  friend void RemoveZeroes(BigInt& obj);
  void Fill(std::vector<BigInt>& powers, std::vector<int>& pow_of_two,
            BigInt u_rhs, BigInt& mod);
  friend std::pair<BigInt, BigInt> Divide(const BigInt& lhs, const BigInt& rhs);
};
BigInt Pow(uint16_t val, uint32_t pow);
BigInt operator+(const BigInt& lhs, const BigInt& rhs);
BigInt operator-(const BigInt& lhs, const BigInt& rhs);
BigInt operator*(const BigInt& lhs, const BigInt& rhs);
BigInt operator/(const BigInt& lhs, const BigInt& rhs);
BigInt operator%(const BigInt& lhs, const BigInt& rhs);
bool operator>(const BigInt& lhs, const BigInt& rhs);
bool operator<=(const BigInt& lhs, const BigInt& rhs);
bool operator>=(const BigInt& lhs, const BigInt& rhs);
bool operator!=(const BigInt& lhs, const BigInt& rhs);
