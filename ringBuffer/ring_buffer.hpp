#pragma once
#include <cstddef>

struct Node {
  int val;
  Node* list;
};
class RingBuffer {
 private:
  Node* head_;
  Node* tail_;
  size_t size_;
  size_t capacity_;

 public:
  explicit RingBuffer(size_t capacity) {
    capacity_ = capacity;
    size_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
  }
  ~RingBuffer() { Clear(); }
  void Clear() {
    size_t s_size = size_;
    int elem;
    for (size_t i = 0; i < s_size; ++i) {
      TryPop(&elem);
    }
  }
  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  bool TryPush(int element) {
    if (size_ == capacity_) {
      return false;
    }
    Node* temp = new Node;
    temp->val = element;
    if (head_ == nullptr) {
      head_ = temp;
    } else {
      tail_->list = temp;
    }
    ++size_;
    tail_ = temp;
    tail_->list = head_;
    return true;
  }
  bool TryPop(int* element) {
    if (size_ == 0) {
      return false;
    }
    if (head_ == nullptr) {
      return false;
    }
    int value;
    if (head_ == tail_) {
      value = head_->val;
      delete head_;
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      struct Node* temp = head_;
      value = temp->val;
      head_ = head_->list;
      tail_->list = head_;
      delete temp;
    }
    *element = value;
    --size_;
    return true;
  }
};
