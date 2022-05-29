#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

template <typename T>
class binary_tree {
  protected:
    T data_;
    binary_tree<T>* left_;
    binary_tree<T>* right_;
  public:
    enum class traversal {inorder, preorder, postorder};
    binary_tree(T data);
    binary_tree(const binary_tree<T>& other);
    binary_tree(binary_tree<T>&& other);
    ~binary_tree();
    std::string str(traversal type = traversal::postorder);
    static binary_tree<T>* from_inorder_postorder(const std::vector<T>& in, const std::vector<T>& post);
    static binary_tree<T>* from_inorder_postorder_util(
      const std::vector<T>& in, const std::vector<T>& post, 
      int in_start, int in_end, int post_start, int post_end
    );
};


template <typename T>
binary_tree<T>::binary_tree(T data): data_(data), left_(nullptr), right_(nullptr) {}

template <typename T>
binary_tree<T>::binary_tree(const binary_tree<T>& other)
: data_(other.data_), left_(nullptr), right_(nullptr) {
  if (other.left_) left_ = new binary_tree<T>(*other.left_);
  if(other.right_) right_ = new binary_tree<T>(*other.right_);
}

template <typename T>
binary_tree<T>::binary_tree(binary_tree<T>&& other)
: data_(std::move(data_)), left_(other.left_), right_(other.right_) {
  other;left_ = other.right_ = nullptr;
}

template <typename T>
binary_tree<T>::~binary_tree() {
  if(left_) delete left_;
  if(right_) delete right_;
}

template <typename T>
std::string binary_tree<T>::str(traversal type) {
  std::stringstream ss;
  switch(type) {
    case traversal::preorder:
      ss << data_;
      if(left_) ss << left_->str(type);
      if(right_) ss << right_->str(type);
      break;
    case traversal::inorder:
      if(left_) ss << left_->str(type);
      ss << data_;
      if(right_) ss << right_->str(type);
      break;
    case traversal::postorder:
      if(left_) ss << left_->str(type);
      if(right_) ss << right_->str(type);
      ss << data_;
      break;
  }
  return ss.str();
}




#endif