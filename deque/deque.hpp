#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class Deque {
 public:
  Deque();
  Deque(size_t count);
  Deque(size_t count, const T& value);
  Deque(const Deque<T>& other_deq);
  ~Deque();
  void resize();
  Deque<T>& operator=(const Deque& other);
  size_t size() const;
  bool empty() const;
  T& operator[](size_t index);
  const T& operator[](size_t index) const;
  T& at(size_t index);
  const T& at(size_t index) const;
  void push_back(const T& elem);
  void pop_back();
  void push_front(const T& elem);
  void pop_front();
  template <bool IsConst>
  class CommonIterator
      : public std::iterator<std::random_access_iterator_tag,
                             std::conditional_t<IsConst, const T, T>> {
    std::conditional_t<IsConst, const T*, T*> ptr_;
    size_t shift_;
    std::vector<T*> data_;
    const size_t kChunkSize = 6;

   public:
    CommonIterator();
    CommonIterator(
        std::conditional_t<IsConst, const T*, T*> ptr, size_t shift,
        std::conditional_t<IsConst, const std::vector<T*>&, std::vector<T*>&>
            vec);
    CommonIterator(const CommonIterator& itor);
    CommonIterator& operator=(const CommonIterator& itor);
    // Dereference operator
    std::conditional_t<IsConst, const T&, T&> operator*();
    // Pre-increment operator
    CommonIterator& operator++();
    CommonIterator operator++(int val);
    CommonIterator& operator--();
    CommonIterator operator--(int val);
    CommonIterator& operator+=(int val);
    CommonIterator& operator-=(int val);
    CommonIterator operator+(int val) const;
    CommonIterator operator-(int val) const;
    std::conditional_t<IsConst, const T*, T*> operator->() const;
    bool operator<(const CommonIterator& other) const;
    bool operator>(const CommonIterator& other) const;
    // Equality operator
    bool operator==(const CommonIterator& other) const;
    // Inequality operator
    bool operator!=(const CommonIterator& other) const;
    bool operator<=(const CommonIterator& other) const;
    bool operator>=(const CommonIterator& other) const;
    std::ptrdiff_t operator-(const CommonIterator& itor) const;
    size_t shift();
  };
  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  iterator begin() {
    if (empty()) {
      return iterator();
    }
    return iterator(&data_[shift_ / kChunkSize][shift_ % kChunkSize], shift_,
                    data_);
  }
  iterator end() {
    if (empty()) {
      return iterator();
    }
    return iterator(
        &data_[(size_ + shift_) / kChunkSize][(size_ + shift_) % kChunkSize],
        size_ + shift_, data_);
  }
  const_iterator cbegin() {
    if (empty()) {
      return const_iterator();
    }
    return const_iterator(&data_[shift_ / kChunkSize][shift_ % kChunkSize],
                          shift_, data_);
  }
  const_iterator cend() {
    if (empty()) {
      return const_iterator();
    }
    return const_iterator(
        &data_[(size_ + shift_) / kChunkSize][(size_ + shift_) % kChunkSize],
        size_ + shift_, data_);
  }
  reverse_iterator rbegin() {
    if (empty()) {
      return reverse_iterator(iterator());
    }
    return reverse_iterator(iterator(
        &data_[(size_ + shift_) / kChunkSize][(size_ + shift_) % kChunkSize],
        size_ + shift_, data_));
  }
  reverse_iterator rend() {
    if (empty()) {
      return reverse_iterator(iterator());
    }
    return reverse_iterator(iterator(
        &data_[(shift_) / kChunkSize][(shift_) % kChunkSize], shift_, data_));
  }
  const_reverse_iterator crbegin() {
    if (empty()) {
      return const_reverse_iterator(const_iterator());
    }
    return const_reverse_iterator(const_iterator(
        &data_[(size_ + shift_) / kChunkSize][(size_ + shift_) % kChunkSize],
        size_ + shift_, data_));
  }
  const_reverse_iterator crend() {
    if (empty()) {
      return const_reverse_iterator(const_iterator());
    }
    return const_reverse_iterator(const_iterator(
        &data_[(shift_) / kChunkSize][(shift_) % kChunkSize], shift_, data_));
  }
  void insert(iterator itor, const T& elem) {
    size_t idor = itor.shift() - shift_;
    push_back(elem);
    idor += shift_;
    for (size_t i = shift_ + size_ - 1; i > idor; --i) {
      data_[i / kChunkSize][i % kChunkSize] =
          data_[(i - 1) / kChunkSize][(i - 1) % kChunkSize];
    }
    data_[idor / kChunkSize][idor % kChunkSize] = elem;
  }
  void erase(iterator itor) {
    size_t idor = itor.shift();
    for (size_t i = idor; i < shift_ + size_ - 1; ++i) {
      data_[i / kChunkSize][i % kChunkSize] =
          data_[(i + 1) / kChunkSize][(i + 1) % kChunkSize];
    }
    size_ -= 1;
  }

 private:
  std::vector<T*> data_;
  size_t shift_;
  size_t size_;
  const size_t kChunkSize = 6;
};
template <typename T>
Deque<T>::Deque() : shift_(0), size_(0) {}
template <typename T>
Deque<T>::Deque(size_t count) {
  size_ = count;
  count += ((kChunkSize - count % kChunkSize) % kChunkSize);
  data_.resize(3 * (count / kChunkSize), nullptr);
  shift_ = count / 2;
  for (size_t i = 0; i < data_.size(); ++i) {
    data_[i] = reinterpret_cast<T*>(new T[kChunkSize * sizeof(T)]);
  }
  for (size_t i = shift_; i < shift_ + size_; ++i) {
    new (&data_[i / kChunkSize][i % kChunkSize]) T(0);
  }
}
template <typename T>
Deque<T>::Deque(size_t count, const T& value) {
  size_ = count;
  size_t copy_count = count;
  copy_count = count + ((kChunkSize - count % kChunkSize) % kChunkSize);
  data_.resize(3 * (copy_count / kChunkSize), nullptr);
  shift_ = copy_count / 2;
  for (size_t i = 0; i < data_.size(); ++i) {
    data_[i] = reinterpret_cast<T*>(new uint8_t[kChunkSize * sizeof(T)]);
  }
  for (size_t i = shift_; i < shift_ + size_; ++i) {
    try {
      new (&data_[i / kChunkSize][i % kChunkSize]) T(value);
    } catch (...) {
      for (size_t j = shift_; j < i; ++j) {
        (&data_[j / kChunkSize][j % kChunkSize])->~T();
      }
      for (size_t j = 0; j < data_.size(); ++j) {
        delete[] reinterpret_cast<uint8_t*>(data_[j]);
      }
      throw;
    }
  }
}
template <typename T>
Deque<T>::Deque(const Deque<T>& other_deq) {
  size_t count = other_deq.data_.size();
  size_ = other_deq.size_;
  data_.resize(count, nullptr);
  shift_ = other_deq.shift_;
  for (size_t i = 0; i < data_.size(); ++i) {
    data_[i] = reinterpret_cast<T*>(new uint8_t[kChunkSize * sizeof(T)]);
  }
  for (size_t i = shift_; i < shift_ + size_; ++i) {
    try {
      new (&data_[i / kChunkSize][i % kChunkSize])
          T(other_deq.data_[i / kChunkSize][i % kChunkSize]);
    } catch (...) {
      for (size_t j = shift_; j < i; ++j) {
        (&data_[j / kChunkSize][j % kChunkSize])->~T();
      }
      for (size_t j = 0; j < data_.size(); ++j) {
        delete[] reinterpret_cast<uint8_t*>(data_[j]);
      }
      throw;
    }
  }
}
template <typename T>
Deque<T>::~Deque() {
  for (size_t i = shift_; i < shift_ + size_; ++i) {
    (&data_[i / kChunkSize][i % kChunkSize])->~T();
  }
  for (size_t i = 0; i < data_.size(); ++i) {
    delete[] reinterpret_cast<uint8_t*>(data_[i]);
  }
}
template <typename T>
void Deque<T>::resize() {
  std::vector<T*> temp(3 * data_.size(), nullptr);
  for (size_t i = 0; i < data_.size(); ++i) {
    temp[i] = reinterpret_cast<T*>(new uint8_t[kChunkSize * sizeof(T)]);
  }
  for (size_t i = 0; i < data_.size(); ++i) {
    temp[i + data_.size()] = data_[i];
    data_[i] = nullptr;
  }
  for (size_t i = 2 * data_.size(); i < temp.size(); ++i) {
    temp[i] = reinterpret_cast<T*>(new uint8_t[kChunkSize * sizeof(T)]);
  }
  shift_ += data_.size() * kChunkSize;
  data_.clear();
  data_.resize(temp.size(), nullptr);
  for (size_t i = 0; i < temp.size(); ++i) {
    data_[i] = temp[i];
    temp[i] = nullptr;
  }
}
template <typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& other) {
  Deque<T> temp = other;
  if (!empty()) {
    for (size_t i = shift_; i < shift_ + size_; ++i) {
      (&data_[i / kChunkSize][i % kChunkSize])->~T();
    }
    for (size_t i = 0; i < data_.size(); ++i) {
      delete[] reinterpret_cast<uint8_t*>(data_[i]);
    }
    data_.clear();
  }
  data_.resize(temp.data_.size(), nullptr);
  for (size_t i = 0; i < temp.data_.size(); ++i) {
    data_[i] = temp.data_[i];
    temp.data_[i] = nullptr;
  }
  shift_ = temp.shift_;
  size_ = temp.size_;
  return *this;
}
template <typename T>
size_t Deque<T>::size() const {
  return size_;
}
template <typename T>
bool Deque<T>::empty() const {
  return size_ == 0;
}
template <typename T>
T& Deque<T>::operator[](size_t index) {
  return data_[(shift_ + index) / kChunkSize][(shift_ + index) % kChunkSize];
}
template <typename T>
const T& Deque<T>::operator[](size_t index) const {
  return data_[(shift_ + index) / kChunkSize][(shift_ + index) % kChunkSize];
}
template <typename T>
T& Deque<T>::at(size_t index) {
  if (index >= size_) {
    throw std::out_of_range("");
  }
  return data_[(shift_ + index) / kChunkSize][(shift_ + index) % kChunkSize];
}
template <typename T>
const T& Deque<T>::at(size_t index) const {
  if (index >= size_) {
    throw std::out_of_range("");
  }
  return data_[(shift_ + index) / kChunkSize][(shift_ + index) % kChunkSize];
}
template <typename T>
void Deque<T>::push_back(const T& elem) {
  if (empty()) {
    *this = Deque(1, elem);
    return;
  }
  if (shift_ + size_ + 1 == data_.size() * kChunkSize) {
    resize();
  }
  size_t idor = size_ + shift_;
  new (&data_[idor / kChunkSize][idor % kChunkSize]) T(elem);
  ++size_;
}
template <typename T>
void Deque<T>::pop_back() {
  (&data_[(size_ + shift_) / kChunkSize][(size_ + shift_) % kChunkSize])->~T();
  --size_;
}
template <typename T>
void Deque<T>::push_front(const T& elem) {
  if (empty()) {
    Deque(1, elem);
    return;
  }
  if (shift_ == 0) {
    resize();
  }
  shift_ -= 1;
  new (&data_[shift_ / kChunkSize][shift_ % kChunkSize]) T(elem);
  ++size_;
}
template <typename T>
void Deque<T>::pop_front() {
  (&data_[(shift_) / kChunkSize][shift_ % kChunkSize])->~T();
  ++shift_;
  --size_;
}
template <typename T>
template <bool IsConst>
Deque<T>::CommonIterator<IsConst>::CommonIterator()
    : ptr_(nullptr), shift_(0) {}
template <typename T>
template <bool IsConst>
Deque<T>::CommonIterator<IsConst>::CommonIterator(
    std::conditional_t<IsConst, const T*, T*> ptr, size_t shift,
    std::conditional_t<IsConst, const std::vector<T*>&, std::vector<T*>&> vec) {
  ptr_ = ptr;
  shift_ = shift;
  data_ = vec;
}
template <typename T>
template <bool IsConst>
Deque<T>::CommonIterator<IsConst>::CommonIterator(
    const CommonIterator<IsConst>& itor) {
  ptr_ = itor.ptr_;
  shift_ = itor.shift_;
  data_ = itor.data_;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>&
Deque<T>::CommonIterator<IsConst>::operator=(const CommonIterator& itor) {
  ptr_ = itor.ptr_;
  shift_ = itor.shift_;
  data_ = itor.data_;
  return *this;
}
template <typename T>
template <bool IsConst>
typename ::std::conditional_t<IsConst, const T&, T&>
Deque<T>::CommonIterator<IsConst>::operator*() {
  return *ptr_;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>&
Deque<T>::CommonIterator<IsConst>::operator++() {
  shift_ += 1;
  ptr_ = &data_[shift_ / kChunkSize][shift_ % kChunkSize];
  return *this;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>
Deque<T>::CommonIterator<IsConst>::operator++(int val) {
  CommonIterator<IsConst> tmp = *this;
  val = 0;
  shift_ += 1 + val;
  ptr_ = &data_[shift_ / kChunkSize][shift_ % kChunkSize];
  return tmp;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>&
Deque<T>::CommonIterator<IsConst>::operator--() {
  shift_ -= 1;
  ptr_ = &data_[shift_ / kChunkSize][shift_ % kChunkSize];
  return *this;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>
Deque<T>::CommonIterator<IsConst>::operator--(int val) {
  CommonIterator<IsConst> tmp = *this;
  val = 0;
  shift_ -= 1 + val;
  ptr_ = &data_[shift_ / kChunkSize][shift_ % kChunkSize];
  return tmp;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>&
Deque<T>::CommonIterator<IsConst>::operator+=(int val) {
  shift_ += val;
  ptr_ = &data_[shift_ / kChunkSize][shift_ % kChunkSize];
  return *this;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>&
Deque<T>::CommonIterator<IsConst>::operator-=(int val) {
  shift_ -= val;
  ptr_ = &data_[shift_ / kChunkSize][shift_ % kChunkSize];
  return *this;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>
Deque<T>::CommonIterator<IsConst>::operator+(int val) const {
  if (ptr_ == nullptr) {
    return *this;
  }
  CommonIterator<IsConst> tmp = *this;
  tmp.shift_ += val;
  tmp.ptr_ = &data_[tmp.shift_ / kChunkSize][tmp.shift_ % kChunkSize];
  return tmp;
}
template <typename T>
template <bool IsConst>
typename Deque<T>::template CommonIterator<IsConst>
Deque<T>::CommonIterator<IsConst>::operator-(int val) const {
  if (ptr_ == nullptr) {
    return *this;
  }
  CommonIterator<IsConst> tmp = *this;
  tmp.shift_ -= val;
  tmp.ptr_ = &data_[tmp.shift_ / kChunkSize][tmp.shift_ % kChunkSize];
  return tmp;
}
template <typename T>
template <bool IsConst>
std::conditional_t<IsConst, const T*, T*>
Deque<T>::CommonIterator<IsConst>::operator->() const {
  return ptr_;
}
template <typename T>
template <bool IsConst>
bool Deque<T>::CommonIterator<IsConst>::operator<(
    const CommonIterator<IsConst>& other) const {
  return ptr_ < other.ptr_;
}
template <typename T>
template <bool IsConst>
bool Deque<T>::CommonIterator<IsConst>::operator>(
    const CommonIterator<IsConst>& other) const {
  return other.ptr_ < ptr_;
}
template <typename T>
template <bool IsConst>
bool Deque<T>::CommonIterator<IsConst>::operator==(
    const CommonIterator<IsConst>& other) const {
  return !(ptr_ < other.ptr_) && !(other.ptr_ < ptr_);
}
template <typename T>
template <bool IsConst>
bool Deque<T>::CommonIterator<IsConst>::operator!=(
    const CommonIterator<IsConst>& other) const {
  return !(ptr_ == other.ptr_);
}
template <typename T>
template <bool IsConst>
bool Deque<T>::CommonIterator<IsConst>::operator<=(
    const CommonIterator<IsConst>& other) const {
  return !(ptr_ > other.ptr_);
}
template <typename T>
template <bool IsConst>
bool Deque<T>::CommonIterator<IsConst>::operator>=(
    const CommonIterator<IsConst>& other) const {
  return !(ptr_ < other.ptr_);
}
template <typename T>
template <bool IsConst>
std::ptrdiff_t Deque<T>::CommonIterator<IsConst>::operator-(
    const CommonIterator<IsConst>& itor) const {
  return shift_ - itor.shift_;
}
template <typename T>
template <bool IsConst>
size_t Deque<T>::CommonIterator<IsConst>::shift() {
  return shift_;
}
