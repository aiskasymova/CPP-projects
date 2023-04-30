#include "big_integer.hpp"
const int kTen = 10;
const int kNine = 9;
BigInt::BigInt() : sign_(1) {}
BigInt::BigInt(int64_t big_num) {
  sign_ = (big_num < 0) ? -1 : 1;
  if (big_num == 0) {
    sign_ = 0;
  }
  std::string str_big_num = std::to_string(big_num);
  if (sign_ == -1) {
    str_big_num.erase(0, 1);
  }
  for (int i = static_cast<int>(str_big_num.length()) - 1; i >= 0; --i) {
    digits_.push_back(str_big_num[i] - '0');
  }
}
BigInt::BigInt(std::string str) {
  if (str[0] == '-' && str[1] == '0') {
    sign_ = 0;
    digits_.push_back(0);
    return;
  }
  if (str[0] == '-') {
    sign_ = -1;
  } else if (str[0] != '0') {
    sign_ = 1;
  } else {
    sign_ = 0;
  }
  if (sign_ == -1) {
    for (size_t i = str.length() - 1; i > 0; --i) {
      digits_.push_back(str[i] - '0');
    }
  } else {
    for (int i = static_cast<int>(str.length()) - 1; i >= 0; --i) {
      digits_.push_back(str[i] - '0');
    }
  }
}
BigInt::BigInt(const BigInt& obj) {
  sign_ = obj.sign_;
  digits_ = obj.digits_;
}
BigInt& BigInt::operator=(const BigInt& obj) {
  sign_ = obj.sign_;
  digits_ = obj.digits_;
  return *this;
}
bool operator<(const BigInt& lhs, const BigInt& rhs) {
  if (lhs.sign_ == -1 && rhs.sign_ == 1) {
    return true;
  }
  if (lhs.sign_ == 1 && rhs.sign_ == -1) {
    return false;
  }
  int lhs_size = static_cast<int>(lhs.digits_.size());
  int rhs_size = static_cast<int>(rhs.digits_.size());
  if (lhs_size != rhs_size && lhs.sign_ == 1) {
    return lhs_size < rhs_size;
  }
  if (lhs_size != rhs_size && lhs.sign_ == -1) {
    return lhs_size > rhs_size;
  }
  for (int i = lhs_size - 1; i >= 0; --i) {
    if (lhs.digits_[i] != rhs.digits_[i]) {
      if (lhs.sign_ == 1) {
        return lhs.digits_[i] < rhs.digits_[i];
      }
      return lhs.digits_[i] > rhs.digits_[i];
    }
  }
  return false;
}
BigInt& BigInt::Plus(const BigInt& rhs) {
  size_t lhs_size = digits_.size();
  size_t rhs_size = rhs.digits_.size();
  int extra = 0;
  int old_extra = extra;
  int main;
  if (rhs_size < lhs_size) {
    for (size_t i = 0; i < rhs_size; ++i) {
      main = (digits_[i] + rhs.digits_[i] + old_extra) % kTen;
      extra = (digits_[i] + rhs.digits_[i] + old_extra) / kTen;
      digits_[i] = main;
      old_extra = extra;
      if ((i == rhs_size - 1) && old_extra == 1) {
        Update(i + 1, true);
      }
    }
  } else {
    for (size_t i = 0; i < lhs_size; ++i) {
      main = (digits_[i] + rhs.digits_[i] + old_extra) % kTen;
      extra = (digits_[i] + rhs.digits_[i] + old_extra) / kTen;
      digits_[i] = main;
      old_extra = extra;
      if (i == lhs_size - 1 && lhs_size == rhs_size && old_extra != 0) {
        digits_.push_back(1);
      } else if (i == lhs_size) {
        Update(rhs, i + 1, old_extra, true);
      }
    }
  }
  return *this;
}
void BigInt::Update(size_t pos, bool flag) {
  int tmp;
  int old_extra = 1;
  for (size_t j = pos; j < digits_.size(); ++j) {
    if (flag) {
      tmp = digits_[j] + old_extra;
      old_extra = (tmp / kTen != 0) ? 1 : 0;
      digits_[j] = (tmp / kTen == 0) ? tmp : 0;
    } else {
      tmp = digits_[j] - old_extra;
      old_extra = (tmp < 0) ? 1 : 0;
      digits_[j] = (tmp < 0) ? kNine : tmp;
    }
  }
}
void BigInt::Update(const BigInt& other, size_t pos, int extra, bool flag) {
  int tmp1;
  int tmp2;
  if (extra == 0) {
    for (size_t j = pos; j < other.digits_.size(); ++j) {
      digits_.push_back(other.digits_[j]);
    }
    return;
  }
  if (flag) {
    for (size_t j = pos; j < other.digits_.size(); ++j) {
      tmp1 = other.digits_[j] + extra;
      tmp2 = (tmp1 / kTen == 1) ? 0 : tmp1;
      extra = (tmp1 / kTen == 1) ? 1 : 0;
      digits_.push_back(tmp2);
    }
  } else {
    for (size_t j = pos; j < other.digits_.size(); ++j) {
      tmp1 = other.digits_[j] - extra;
      tmp2 = (tmp1 < 0) ? kNine : tmp1;
      extra = (tmp1 < 0) ? 1 : 0;
      digits_.push_back(tmp2);
    }
  }
}
BigInt& BigInt::Minus(const BigInt& rhs) {
  int lhs_size = static_cast<int>(digits_.size());
  int rhs_size = static_cast<int>(rhs.digits_.size());
  if (lhs_size == rhs_size && rhs_size == 1) {
    sign_ = (digits_[0] < rhs.digits_[0]) ? rhs.sign_ : sign_;
    digits_[0] = std::max(digits_[0], rhs.digits_[0]) -
                 std::min(digits_[0], rhs.digits_[0]);
    return (*this);
  }
  if (rhs_size <= lhs_size) {
    MinusLess(rhs, rhs_size);
  } else {
    MinusGreater(rhs, lhs_size);
  }
  return *this;
}
BigInt& BigInt::MinusLess(const BigInt& rhs, int size) {
  int extra;
  int old_extra = 0;
  int tmp;
  int tmp1;
  for (int i = 0; i < size; ++i) {
    tmp1 = digits_[i] - old_extra;
    tmp = tmp1 - rhs.digits_[i];
    extra = (tmp < 0) ? 1 : 0;
    digits_[i] = (tmp < 0) ? ((kTen + tmp1) - rhs.digits_[i]) : tmp;
    old_extra = extra;
    if (old_extra != 0 && i == size - 1) {
      Update(i + 1, false);
    }
  }
  return *this;
}
BigInt& BigInt::MinusGreater(const BigInt& rhs, int size) {
  int extra;
  int old_extra = 0;
  int tmp;
  int tmp1;
  sign_ = rhs.sign_;
  for (int i = 0; i < size; ++i) {
    tmp = rhs.digits_[i] - old_extra - digits_[i];
    extra = (tmp < 0) ? 1 : 0;
    digits_[i] =
        (tmp < 0) ? ((kTen + rhs.digits_[i] - old_extra) - digits_[i]) : tmp;
    old_extra = extra;
    if (i == size - 1) {
      Update(rhs, i + 1, old_extra, false);
    }
  }
  return *this;
}
BigInt MyMinus(const BigInt& lhs, const BigInt& rhs) {
  BigInt ans = rhs;
  ans.Minus(lhs);
  return ans;
}
BigInt& BigInt::operator+=(const BigInt& other) {
  if (sign_ == other.sign_) {
    Plus(other);
  } else {
    Minus(other);
  }
  return (*this);
}
BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
  BigInt sum = lhs;
  sum += rhs;
  return sum;
}
BigInt& BigInt::operator-=(const BigInt& other) {
  if (sign_ == 0) {
    *this = other;
    sign_ = -1 * other.sign_;
    return *this;
  }
  if (sign_ == -1 && other.sign_ == -1) {
    if (other < (*this)) {
      (*this) = MyMinus((*this), other);
      sign_ = 1;
    } else {
      Minus(other);
    }
    return (*this);
  }
  if (sign_ == 1 && other.sign_ == -1) {
    return Plus(other);
  }
  if (sign_ == 1 && other.sign_ == 1) {
    if ((*this) < other) {
      (*this) = MyMinus((*this), other);
      sign_ = -1;
      return (*this);
    }
    return Minus(other);
  }
  if (sign_ == -1 && other.sign_ == 1) {
    return Plus(other);
  }
  return (*this);
}
BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
  BigInt res = lhs;
  res -= rhs;
  return res;
}
void BigInt::Helper(const std::vector<int>& vec, int val) {
  int cnt = static_cast<int>(val);
  while (cnt != 0 && vec[cnt] == 0) {
    digits_.pop_back();
    --cnt;
  }
}
BigInt& BigInt::operator*=(const BigInt& other) {
  if ((digits_[0] == 0 && digits_.size() == 1) ||
      (other.digits_[0] == 0 && other.digits_.size() == 1)) {
    *this = BigInt(0);
    return *this;
  }
  sign_ = sign_ * other.sign_;
  size_t lhs_size = digits_.size();
  size_t rhs_size = other.digits_.size();
  std::vector<int> vec(lhs_size + rhs_size, 0);
  for (size_t i = 0; i < lhs_size; ++i) {
    for (size_t j = 0; j < rhs_size; ++j) {
      vec[i + j] += (digits_[i]) * (other.digits_[j]);
    }
  }
  int tmp;
  int extra = 0;
  size_t full_size = lhs_size + rhs_size;
  digits_.resize(vec.size());
  for (size_t i = 0; i < full_size; ++i) {
    tmp = extra + vec[i];
    vec[i] = tmp % kTen;
    extra = tmp / kTen;
    digits_[i] = vec[i];
  }
  Helper(vec, full_size - 1);
  return *this;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
  BigInt product = lhs;
  product *= rhs;
  return product;
}
void RemoveZeroes(BigInt& obj) {
  size_t size = obj.digits_.size();
  int cnt = static_cast<int>(size - 1);
  while (obj.digits_[cnt] == 0) {
    obj.digits_.pop_back();
    --cnt;
  }
}
BigInt Pow(uint16_t val, uint32_t pow) {
  if (pow == 0) {
    return BigInt(1);
  }
  BigInt ans(val);
  uint32_t i_cnt = 2;
  while (i_cnt <= pow) {
    ans *= 2;
    ++i_cnt;
  }
  return ans;
}
void BigInt::Fill(std::vector<BigInt>& powers, std::vector<int>& pow_of_two,
                  BigInt u_rhs, BigInt& mod) {
  int pow_size = static_cast<int>(powers.size());
  pow_of_two.push_back(pow_size - 1);
  int i_size = pow_size - 2;
  int j_ind;
  while (*this > u_rhs) {
    j_ind = i_size;
    while (powers[j_ind] >= *this) {
      --j_ind;
    }
    *this -= powers[j_ind];
    pow_of_two.push_back(j_ind);
    i_size = j_ind;
    RemoveZeroes(*this);
    if (*this < u_rhs) {
      mod = *this;
    }
  }
}
std::pair<BigInt, BigInt> Divide(const BigInt& lhs, const BigInt& rhs) {
  std::vector<BigInt> powers;
  BigInt current = rhs;
  current.sign_ = 1;
  BigInt u_lhs = lhs;
  u_lhs.sign_ = 1;
  BigInt u_rhs = rhs;
  u_rhs.sign_ = 1;
  while (current <= u_lhs) {
    powers.push_back(current);
    current *= 2;
  }
  std::vector<int> pow_of_two;
  BigInt tmp = lhs - powers[powers.size() - 1];
  BigInt mod = 0;
  if (tmp < u_rhs && tmp.digits_[0] != 0) {
    mod = 1;
  }
  tmp.Fill(powers, pow_of_two, u_rhs, mod);
  BigInt ans(0);
  BigInt two;
  for (size_t j = 0; j < pow_of_two.size(); ++j) {
    two = Pow(2, pow_of_two[j]);
    ans += two;
  }
  ans.sign_ = lhs.sign_ * rhs.sign_;
  mod.sign_ = lhs.sign_ * rhs.sign_;
  return std::make_pair(ans, mod);
}
BigInt& BigInt::operator/=(const BigInt& other) {
  BigInt u_lhs = *this;
  u_lhs.sign_ = 1;
  BigInt u_rhs = other;
  u_rhs.sign_ = 1;
  if (u_rhs > u_lhs) {
    *this = BigInt(0);
    return *this;
  }
  if (u_lhs == u_rhs) {
    *this = BigInt(1);
    return *this;
  }
  if (other == 1) {
    return *this;
  }
  *this = Divide(*this, other).first;
  return *this;
}
BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
  BigInt div = lhs;
  div /= rhs;
  return div;
}
BigInt& BigInt::operator%=(const BigInt& other) {
  BigInt u_lhs = *this;
  u_lhs.sign_ = 1;
  BigInt u_rhs = other;
  u_rhs.sign_ = 1;
  if (u_lhs < u_rhs) {
    return *this;
  }
  if (u_lhs == u_rhs) {
    *this = BigInt(0);
    return *this;
  }
  *this = Divide(*this, other).second;
  if (digits_[0] == 0) {
    sign_ = 0;
  }
  return *this;
}
BigInt operator%(const BigInt& lhs, const BigInt& rhs) {
  BigInt mod = lhs;
  mod %= rhs;
  return mod;
}
bool operator==(const BigInt& lhs, const BigInt& rhs) {
  return (lhs.digits_ == rhs.digits_) && (lhs.sign_ == rhs.sign_);
}
bool operator!=(const BigInt& lhs, const BigInt& rhs) { return !(lhs == rhs); }
bool operator>(const BigInt& lhs, const BigInt& rhs) {
  return !(lhs < rhs) && (lhs != rhs);
}
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
  return (lhs < rhs) || (lhs == rhs);
}
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
  return (lhs > rhs) || (lhs == rhs);
}
std::istream& operator>>(std::istream& stream, BigInt& big_int) {
  std::string str;
  stream >> str;
  if (str[0] == '0') {
    big_int.sign_ = 0;
    big_int.digits_[0] = 0;
    return stream;
  }
  if (str[0] == '-') {
    big_int.sign_ = -1;
    str.erase(0, 1);
  } else {
    big_int.sign_ = 1;
  }
  for (int i = static_cast<int>(str.length()) - 1; i >= 0; --i) {
    big_int.digits_.push_back(str[i] - '0');
  }
  return stream;
}

std::ostream& operator<<(std::ostream& out, const BigInt& big_int) {
  if (big_int.sign_ == 0) {
    out << 0;
    return out;
  }
  for (int i = static_cast<int>(big_int.digits_.size()) - 1; i >= 0; --i) {
    if (big_int.sign_ == -1 &&
        i == static_cast<int>(big_int.digits_.size()) - 1) {
      out << -1 * big_int.digits_[i];
    } else {
      out << big_int.digits_[i];
    }
  }
  return out;
}
BigInt& BigInt::operator-() {
  sign_ = -1 * sign_;
  return *this;
}
BigInt& BigInt::operator++() {
  int ind;
  int size = static_cast<int>(digits_.size());
  if (sign_ == 1) {
    for (ind = 0; (ind < size) && (digits_[ind] == kNine); ++ind) {
      digits_[ind] = 0;
    }
    if (ind == size) {
      digits_.push_back(1);
    } else {
      ++digits_[ind];
    }
  } else if (sign_ == -1) {
    for (ind = 0; (ind < size) && (digits_[ind] == 0); ++ind) {
      digits_[ind] = kNine;
    }
    if (ind == size) {
      digits_.push_back(1);
    } else {
      --digits_[ind];
    }
    if (digits_[0] == 0) {
      sign_ = 0;
    }
  } else {
    ++digits_[0];
    sign_ = 1;
  }
  return *this;
}
BigInt BigInt::operator++(int) {
  BigInt aux;
  aux = *this;
  ++(*this);
  return aux;
}

BigInt& BigInt::operator--() {
  if (digits_[0] == 1 && digits_.size() == 1) {
    sign_ = 0;
    digits_[0] = 0;
    return *this;
  }
  int ind;
  int size = static_cast<int>(digits_.size());
  if (digits_[0] == 0 && digits_.size() == 1) {
    sign_ = -1;
    ++digits_[0];
    return *this;
  }
  for (ind = 0; digits_[ind] == 0 && ind < size; ++ind) {
    digits_[ind] = kNine;
  }
  int tmp = digits_[ind] - 1;
  digits_[ind] = (tmp < 0) ? (-1 * tmp) : tmp;
  if (size > 1 && digits_[size - 1] == 0) {
    digits_.pop_back();
  }
  return *this;
}
BigInt BigInt::operator--(int) {
  BigInt aux;
  aux = *this;
  --(*this);
  return aux;
}
