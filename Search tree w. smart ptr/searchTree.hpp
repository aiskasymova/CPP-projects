#include <iostream>
#include <memory>

template <typename T>
class Tree {
 public:
  virtual void insert(const T& data) = 0;

  virtual void insert(T&& data) = 0;

  virtual void remove() = 0;

  virtual void remove(T key) = 0;

  virtual T& min() = 0;

  virtual T& max() = 0;

  virtual bool hasElem(const T& data) const = 0;  // NOLINT

  virtual ~Tree() = default;
};

template <typename T>
class BinaryTree : public Tree<T> {
 private:
  struct Node {
    T data;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::weak_ptr<Node> parent;

    Node(const T& value) : data(value) {}
    ~Node() = default;
  };

  std::shared_ptr<Node> root_;

 public:
  BinaryTree() : root_(nullptr) {}

  BinaryTree(const BinaryTree& other) {
    if (other.root_ != nullptr) {
      root_ = std::make_shared<Node>(other.root_->data);
      copyTree(root_, other.root_);  // NOLINT
    }
  }

  BinaryTree(BinaryTree&& other) noexcept : root_(std::move(other.root_)) {}
  ~BinaryTree() { root_.reset(); }

  BinaryTree& operator=(const BinaryTree& other) {
    if (this != &other) {
      root_.reset();
      if (other.root_) {
        root_ = std::make_shared<Node>(other.root_->data);
        copyTree(root_, other.root_);  // NOLINT
      }
    }
    return *this;
  }

  void insert(const T& data) override {
    if (!root_) {
      root_ = std::make_shared<Node>(data);
    } else {
      insertHelper(root_, data);  // NOLINT
    }
  }

  void insert(T&& value) override {
    if (!root_) {
      root_ = std::make_shared<Node>(std::move(value));
    } else {
      insertHelper(root_, std::move(value));  // NOLINT
    }
  }

  void remove() override {
    if (root_) {
      remove(root_->data);
    }
  }

  void remove(T key) override {
    if (root_) {
      std::shared_ptr<Node> node_to_del = getNode(root_, key);  // NOLINT
      deleteNode(node_to_del);                                  // NOLINT
    }
  }

  T& min() override {
    if (!root_) {
      throw std::runtime_error("Tree is empty in min");
    }
    return findMin(root_)->data;  // NOLINT
  }

  T& max() override {
    if (!root_) {
      throw std::runtime_error("Tree is empty in max");
    }
    return findMax(root_)->data;  // NOLINT
  }

  bool hasElem(const T& data) const override {  // NOLINT
    return search(root_, data);
  }

  std::shared_ptr<Node> getNode(const std::shared_ptr<Node>& node,  // NOLINT
                                T key) {
    if (key == node->data) {
      return node;
    }
    if (key < node->data) {
      return getNode(node->left, key);  // NOLINT
    }
    return getNode(node->right, key);  // NOLINT
  }

  T top() const {
    if (!root_) {
      throw std::runtime_error("Tree is empty in top");
    }
    return root_->data;
  }

 private:
  void insertHelper(std::shared_ptr<Node>& node, const T& value) {  // NOLINT
    std::shared_ptr<Node> current = node;
    while (current) {
      if (value > current->data) {
        if (current->right) {
          current = current->right;
        } else {
          current->right = std::make_shared<Node>(value);
          current->right->parent = current;
          break;
        }
      } else if (value < current->data) {
        if (current->left) {
          current = current->left;
        } else {
          current->left = std::make_shared<Node>(value);
          current->left->parent = current;
          break;
        }
      }
    }
    root_ = node;
  }

  std::shared_ptr<Node> findSuccessor(std::shared_ptr<Node> node) {  // NOLINT
    std::shared_ptr<Node> successor = nullptr;
    if (node->right) {
      successor = node->right;
      while (successor->left) {
        successor = successor->left;
      }
    }
    return successor;
  }

  void deleteNode(std::shared_ptr<Node> node_to_del) {  // NOLINT
    if (!root_ || !node_to_del) {
      return;
    }
    std::shared_ptr<Node> parent = node_to_del->parent.lock();
    if (!node_to_del->left && !node_to_del->right) {
      if (parent) {
        if (parent->left == node_to_del) {
          parent->left.reset();
        } else {
          parent->right.reset();
        }
      } else {
        root_.reset();
      }
    } else if (!node_to_del->left || !node_to_del->right) {
      std::shared_ptr<Node> child =
          (node_to_del->left) ? node_to_del->left : node_to_del->right;
      if (parent) {
        if (parent->left == node_to_del) {
          parent->left = child;
        } else {
          parent->right = child;
        }
      } else {
        root_ = child;
      }
      child->parent = parent;
    } else {
      thirdcase(node_to_del);
    }
  }
  void thirdcase(std::shared_ptr<Node> node_to_del) {
    std::shared_ptr<Node> successor = findSuccessor(node_to_del);
    node_to_del->data = successor->data;
    std::shared_ptr<Node> sparent = successor->parent.lock();
    if (sparent) {
      if (sparent->left == successor) {
        sparent->left = successor->right;
      } else {
        sparent->right = successor->right;
      }
      if (successor->right) {
        successor->right->parent = sparent;
      }
    } else {
      root_ = successor->right;
      if (successor->right) {
        successor->right->parent.reset();
      }
    }
  }
  std::shared_ptr<Node> findMin(std::shared_ptr<Node> node) const {  // NOLINT
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  std::shared_ptr<Node> findMax(std::shared_ptr<Node> node) const {  // NOLINT
    while (node->right) {
      node = node->right;
    }
    return node;
  }

  bool search(const std::shared_ptr<Node>& node, const T& data) const {
    std::shared_ptr<Node> current = node;
    bool exp = false;
    while (current) {
      if (data == current->data) {
        exp = true;
        break;
      }
      if (data < current->data) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return exp;
  }

  void copyTree(std::shared_ptr<Node>& dest,  // NOLINT
                const std::shared_ptr<Node>& src) {
    if (src->left) {
      dest->left = std::make_shared<Node>(src->left->data);
      dest->left->parent = dest;
      copyTree(dest->left, src->left);
    }
    if (src->right) {
      dest->right = std::make_shared<Node>(src->right->data);
      dest->right->parent = dest;
      copyTree(dest->right, src->right);
    }
  }
};

