#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 private:
  T MyTrace(const Matrix<N, N, T>& obj);
  std::vector<std::vector<T>> matrix_;

 public:
  Matrix();
  Matrix(std::vector<std::vector<T>>& other);
  Matrix(T elem);
  Matrix& operator+=(const Matrix<N, M, T>& k_other);
  Matrix& operator-=(const Matrix<N, M, T>& k_other);
  Matrix& operator*=(const T& number);
  T& operator()(const size_t& ind1, const size_t& ind2);
  const T& operator()(const size_t& ind1, const size_t& ind2) const;
  T Trace();
  Matrix<M, N, T> Transposed();
};
template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix() {
  matrix_.resize(N, std::vector<T>(M));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(std::vector<std::vector<T>>& other) {
  matrix_ = other;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(T elem) {
  matrix_.resize(N, std::vector<T>(M, elem));
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const Matrix<N, M, T>& matr, const T& number) {
  Matrix<N, M, T> copy = matr;
  copy *= number;
  return copy;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& k_other) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matrix_[i][j] += k_other.matrix_[i][j];
    }
  }
  return *this;
}
template <size_t N, size_t M, size_t U, size_t V, typename T>
bool operator==(const Matrix<N, M, T>& k_lhs, const Matrix<U, V, T>& k_rhs) {
  if (N != U && M != V) {
    return false;
  }
  bool exp = true;
  size_t firstcnt = 0;
  size_t secondcnt = 0;
  while (firstcnt < N) {
    while (secondcnt < U) {
      if (k_lhs(firstcnt, secondcnt) != k_rhs(firstcnt, secondcnt)) {
        exp = false;
        break;
      }
      ++secondcnt;
    }
    ++firstcnt;
  }
  return exp;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& k_other) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matrix_[i][j] -= k_other.matrix_[i][j];
    }
  }
  return *this;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T>& k_lhs,
                          const Matrix<N, M, T>& k_rhs) {
  Matrix<N, M, T> sum = k_lhs;
  sum += k_rhs;
  return sum;
}
template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T>& k_lhs,
                          const Matrix<N, M, T>& k_rhs) {
  Matrix<N, M, T> sum = k_lhs;
  sum -= k_rhs;
  return sum;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const T& number) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      matrix_[i][j] *= number;
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(const size_t& ind1, const size_t& ind2) {
  return matrix_[ind1][ind2];
}

template <size_t N, size_t M, typename T>
const T& Matrix<N, M, T>::operator()(const size_t& ind1,
                                     const size_t& ind2) const {
  return matrix_[ind1][ind2];
}

template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() {
  Matrix<M, N, T> trans;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      trans(i, j) = matrix_[j][i];
    }
  }
  return trans;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::Trace() {
  return MyTrace(*this);
}
template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::MyTrace(const Matrix<N, N, T>& obj) {
  T sum = 0;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      if (i == j) {
        sum += obj(i, j);
      }
    }
  }
  return sum;
}
template <size_t N, size_t M, size_t K, typename T>
Matrix<N, K, T> operator*(const Matrix<N, M, T>& matr1,
                          const Matrix<M, K, T>& matr2) {
  Matrix<N, K, T> res;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < K; ++j) {
      for (size_t k = 0; k < M; ++k) {
        res(i, j) += matr1(i, k) * matr2(k, j);
      }
    }
  }
  return res;
}
